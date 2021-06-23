#include "interpreter.h"
#include "frame.h"
#include "universe.h"
#include "codeObject.h"
#include "stringObject.h"
#include "listObject.h"
#include "dictObject.h"
#include "cellObject.h"
#include "functionObject.h"
#include "typeObject.h"

#include <cassert>
#include <iostream>
#include <iomanip>
using namespace std;

/* Interpreter */

Interpreter::Interpreter(int debug) {
    this->_debug = debug;

    op = new op_t[256];
    for(int i = 0; i < 256; i++)
        op[i] = &Interpreter::unimplemented;
    
    op[1]   = &Interpreter::pop_top;
    op[20]  = &Interpreter::binary_multiply;
    op[21]  = &Interpreter::binary_divide;
    op[22]  = &Interpreter::binary_modulo;
    op[23]  = &Interpreter::binary_add;
    op[24]  = &Interpreter::binary_subtract;
    op[25]  = &Interpreter::binary_subscr;
    op[54]  = &Interpreter::store_map;
    op[60]  = &Interpreter::store_subscr;
    op[61]  = &Interpreter::delete_subscr;
    op[68]  = &Interpreter::get_iter;
    op[71]  = &Interpreter::print_item;
    op[72]  = &Interpreter::print_newline;
    op[80]  = &Interpreter::break_loop;
    op[82]  = &Interpreter::load_locals;
    op[83]  = &Interpreter::return_value;
    op[87]  = &Interpreter::pop_block;
    op[89]  = &Interpreter::build_class;
    op[90]  = &Interpreter::store_name;
    op[92]  = &Interpreter::unpack_sequence;
    op[93]  = &Interpreter::for_iter;
    op[97]  = &Interpreter::store_global;
    op[100] = &Interpreter::load_const;
    op[101] = &Interpreter::load_name;
    op[102] = &Interpreter::build_tuple;
    op[103] = &Interpreter::build_list;
    op[105] = &Interpreter::build_map;
    op[106] = &Interpreter::load_attr;
    op[107] = &Interpreter::compare_op;
    op[110] = &Interpreter::jump_forward;
    op[113] = &Interpreter::jump_absolute;
    op[114] = &Interpreter::pop_jump_if_false;
    op[116] = &Interpreter::load_global;
    op[120] = &Interpreter::setup_loop;
    op[124] = &Interpreter::load_fast;
    op[125] = &Interpreter::store_fast;
    op[131] = &Interpreter::call_function;
    op[132] = &Interpreter::make_function;
    op[134] = &Interpreter::make_closure;
    op[135] = &Interpreter::load_closure;
    op[136] = &Interpreter::load_deref;
    op[137] = &Interpreter::store_deref;

    _frame = nullptr; /* initialize from codeObject when run() */
    _builtins = new DictObject();
    _builtins->put(new StringObject("True"), Universe::True);
    _builtins->put(new StringObject("False"), Universe::False);
    _builtins->put(new StringObject("None"), Universe::None);
    _builtins->put(new StringObject("int"), IntegerKlass::get_instance()->type_object());
    _builtins->put(new StringObject("object"), ObjectKlass::get_instance()->type_object());
    _builtins->put(new StringObject("str"), StringKlass::get_instance()->type_object());
    _builtins->put(new StringObject("list"), ListKlass::get_instance()->type_object());
    _builtins->put(new StringObject("dict"), DictKlass::get_instance()->type_object());
    _builtins->put(new StringObject("len"), new FunctionObject(len));
    _builtins->put(new StringObject("isinstance"), new FunctionObject(isinstance));
    _builtins->put(new StringObject("type"), new FunctionObject(type_of));
}

void Interpreter::run(CodeObject* co) {
    _frame = new Frame(co);
    eval_frame();
    destroy_frame();
}

void Interpreter::push(Object* p) {
    _frame->stack()->append(p);
}

Object* Interpreter::pop() {
    return _frame->stack()->pop();
}

Object* Interpreter::top() {
    Object* tmp = pop();
    push(tmp);
    return tmp;
}

int Interpreter::stack_level() {
    return _frame->stack()->size();
}

void Interpreter::build_frame(Object* callable, ObjList* args, int oparg) {
    /* called in call_function() */
    /* if no args, args is null */
    /* callable is either FunctionObject, NativeFunctionObject or MethodObject */
    if(callable->klass() == NativeFunctionKlass::get_instance()) {
        if(_debug) {
            cerr << "\t<Native Function>" << endl;
        }
        push(((FunctionObject*)callable)->call(args));
    }
    else if(callable->klass() == FunctionKlass::get_instance()) {
        if(_debug) {
            cerr << "\t<Non-Native Function>" << endl;
        }
        Frame* frame = new Frame((FunctionObject*)callable, args, oparg);
        frame->set_sender(_frame);
        _frame = frame;
    }
    else if(callable->klass() == MethodKlass::get_instance()) {
        if(_debug) {
            cerr << "\t<Method>";
        }
        MethodObject* method = (MethodObject*)callable;
        if(args == nullptr) {
            args = new ObjList(equal2obj);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, oparg);
    }
    else if(callable->klass() == TypeKlass::get_instance()) {
        if(_debug) {
            cerr << "\t<type>" << endl;
        }
        Object* instance = ((TypeObject*)callable)->own_klass()->allocate_instance(args);
        //Object* instance = Klass::allocate_instance(callable, args);
        push(instance);
    }
    else {
        cerr << "Error build_frame, unrecognized klass = ";
        callable->klass()->name()->print(); 
        cout << endl;
        assert(0);
    }
}

void Interpreter::eval_frame() {    
    while(_frame->has_more_codes()) {

        /* pc is updated automatically in get_op_code() or get_op_arg() */
        unsigned char op_code = _frame->get_op_code();

        if(_debug)
            cout << "[" << _frame->get_pc() - 1 << "]\t" << (int)op_code << ":\t";

        /* (optional) op-arg */
        int op_arg = -1;
        bool has_arg = op_code >= HAVE_ARGUMENT;
        if(has_arg) {
            op_arg = _frame->get_op_arg();
        }

        if(op[op_code] == &Interpreter::unimplemented) {
            cout << "Unimplemented opcode [" << (int)op_code << "]" << endl;
        }

        /* op */
        (this->*op[op_code])(op_arg);
    }
}

void Interpreter::leave_frame(Object* retv) {
    /* a frame has a return value */
    destroy_frame();
    push(retv);
}

void Interpreter::destroy_frame() {
    /* destroy current frame and return to last frame */
    Frame* tmp = _frame;
    _frame = _frame->sender();
    delete tmp;
}

/* instructions */

void Interpreter::unimplemented(int arg) { 
    cout << "error: unrecognized opcode" << endl;
    exit(-1);
}

/* 0 */

void Interpreter::pop_top(int arg) {
    if(_debug) {
        cerr << "POP_TOP" << endl;
    }
    pop();
}

/* 10 */

/* 20 */

void Interpreter::binary_multiply(int arg) /* 20*/ {
    if(_debug) {
        cerr << "BINARY_MULTIPLY" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->mul(u));
}

void Interpreter::binary_divide(int arg) /* 21 */ {
    if(_debug) {
        cerr << "BINARY_DIVIDE" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->div(u));
}

void Interpreter::binary_modulo(int arg) /* 22 */ {
    if(_debug) {
        cerr << "BINARY_MODULO" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->mod(u));
}

void Interpreter::binary_add(int arg) /* 23 */ {
    if(_debug) {
        cerr << "BINARY_ADD" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->add(u));
}

void Interpreter::binary_subtract(int arg) /* 24 */ {
    if(_debug) {
        cerr << "BINARY_SUBTRACT" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->sub(u));
}

void Interpreter::binary_subscr(int arg) /* 25 */ {
    if(_debug) {
        cerr << "BINARY_SUBSCR" << endl;
    }
    Object* index = pop();
    Object* obj = pop();
    push(obj->subscr(index));
}

/* 30 */

/* 40 */

/* 50 */

void Interpreter::store_map(int arg) /* 54 */ {
    if(_debug) {
        cerr << "STORE_MAP" << endl;
    }
    Object* key = pop();
    Object* value = pop();
    DictObject* dict = (DictObject*)top();
    dict->put(key, value);
}

/* 60 */

void Interpreter::store_subscr(int arg) /* 60 */ {
    if(_debug) {
        cerr << "STORE_SUBSCR" << endl;
    }
    Object* x = pop();
    Object* obj = pop();
    Object* index = pop();
    obj->store_subscr(x, index);
}

void Interpreter::delete_subscr(int arg) /* 61 */ {
    if(_debug) {
        cerr << "DELETE_SUBSCR";
    }
    Object* index = pop();
    Object* obj = pop();
    obj->del_subscr(index);
}

void Interpreter::get_iter(int arg) /* 68 */ {
    if(_debug) {
        cerr << "GET_ITER" << endl;
    }
    Object* obj = pop();
    push(obj->iter());
}

/* 70 */

void Interpreter::print_item(int arg) /* 71 */ {
    if(_debug) {
        cerr << "PRINT_ITEM | ";
    }
    Object* obj = pop();
    obj->print();
    if(_debug) {
        cerr << endl;
    }
}

void Interpreter::print_newline(int arg) /* 72 */ {
    if(_debug) {
        cerr << "PRINT_NEWLINE" << endl;
    }
    cout << endl;
}

/* 80 */

void Interpreter::break_loop(int arg) /* 80 */ {
    if(_debug) {
        cerr << "BREAK_LOOP" << endl;
    }
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    _frame->set_pc(lb->_target);
}

void Interpreter::load_locals(int arg) /* 82 */ {
    if(_debug) {
        cerr << "LOAD_LOCALS" << endl;
    }
    push(_frame->locals());
}

void Interpreter::return_value(int arg) /* 83 */ {
    if(_debug) {
        cerr << "RETURN_VALUE | ";
    }
    Object* retv = pop();
    if(_debug) {
        retv->klass()->name()->print();
        cout << " ";
        retv->print();
        cout << endl;
    }
    if(_frame->is_first_frame())
        return;
    leave_frame(retv);
}

void Interpreter::pop_block(int arg) /* 87 */ {
    if(_debug) {
        cerr << "POP_BLOCK" << endl;
    }
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
}

void Interpreter::build_class(int arg) /* 89 */ {
    if(_debug) {
        cerr << "BUILD_CLASS" << endl;
    }
    Object* locals_dict, *supers_tuple, *name_str;
    locals_dict = pop();
    supers_tuple = pop();
    name_str = pop();
    TypeObject* type_object = Klass::create_klass(locals_dict, supers_tuple, name_str);
    push(type_object);
}

/* 90 */

void Interpreter::store_name(int arg) /* 90 */ {
    if(_debug) {
        cerr << "STORE_NAME | index = [" << arg << "] | name = \"";
        (_frame->names()->get(arg))->print();
        cerr << "\"" << endl;
    }
    _frame->locals()->put(_frame->names()->get(arg), pop());
}

void Interpreter::unpack_sequence(int arg) /* 92 */ {
    if(_debug) {
        cerr << "UNPACK_SEQUENCE | " << arg << endl;
    }
    Object* seq = pop();
    if(seq->len() != nullptr && ((IntegerObject*)(seq->len()))->value() == arg) {
        while(arg--) {
            push(seq->subscr(new IntegerObject(arg)));
        }
    }
    else {
        cerr << "error unpack sequence" << endl
            << "obj type: " << seq->klass()->name() << endl;
        if(seq->len() == nullptr)
            cerr << "len = NULL" << endl;
        else   
            cerr << "len = " << ((IntegerObject*)(seq->len()))->value() << endl;
        exit(-1);
    }

}

void Interpreter::for_iter(int arg) /* 93 */ {
    if(_debug) {
        cerr << "FOR_ITER" << endl;
    }
    Object* iter = top();
    Object* next_method_obj = iter->getattr(StringTable::get_instance()->str_next);
    /* frame of next() of iterator */
    build_frame(next_method_obj, nullptr, arg);

    if(/* return value of iter.next() */ top() == nullptr) {
        _frame->set_pc(_frame->get_pc() + arg);
        pop();
    }
}

void Interpreter::store_global(int arg) /* 97 */ {
    if(_debug) {
        cerr << "STORE_GLOBAL" << endl;
    }
    _frame->globals()->put(_frame->names()->get(arg), pop());
}

/* 100 */

void Interpreter::load_const(int arg) /* 100 */ {
    if(_debug) {
        cerr << "LOAD_CONST | [" << arg << "] | ";
    }
    Object* v = _frame->consts()->get(arg);
    if(v == nullptr)
        v = Universe::None;
    if(_debug) {
        v->klass()->name()->print();
        cout << ": ";
        v->print();
        cout << endl;
    }
    push(v);
}

void Interpreter::load_name(int arg) /* 101 */ {
    if(_debug) {
        Object* x = _frame->names()->get(arg);
        cerr << "LOAD_NAME | index = [" << arg << "] | name = \"";
        x->print();
        cout << "\" | value = ";
        cout.flush();
    }
    Object* name = _frame->names()->get(arg);
    Object* obj = _frame->locals()->get(name);
    if(obj != Universe::None) {
        if(_debug) {
            obj->klass()->name()->print();
            cout << " ";
            obj->print();
            cout << endl;
        }
        push(obj);
        return;
    }
    obj = _frame->globals()->get(name);
    if(obj != Universe::None) {
        if(_debug) {
            obj->klass()->name()->print();
            cout << " ";
            obj->print();
            cout << endl;
        }
        push(obj);
        return;
    }
    obj = _builtins->get(name);
    if(obj != Universe::None) {
        if(_debug) {
            obj->klass()->name()->print();
            cout << " ";
            obj->print();
            cout << endl;
        }
        push(obj);
        return;
    }
    if(_debug) {
        cout << "None\n";
    }
    push(Universe::None);
}

void Interpreter::build_tuple(int arg) /* 102 */ {
    if(_debug) {
        cerr << "BUILD_TUPLE | [" << arg << "]" << endl;
    }
    Object* tuple = new TupleObject();
    while(arg--) {
        ((TupleObject*)tuple)->set(arg, pop());
    }
    push(tuple);
}

void Interpreter::build_list(int arg) { /* 103 */
    if(_debug) {
        cerr << "BUILD_LIST | [" << arg << "]" << endl;
    }
    Object* lst = new ListObject();
    while(arg--) {
        ((ListObject*)lst)->set(arg, pop());
    }
    push(lst);
}

void Interpreter::build_map(int arg) /* 105 */ {
    if(_debug) {
        cerr << "BUILD_MAP" << endl;
    }
    Object* dict = new DictObject();
    push(dict);    
}

void Interpreter::load_attr(int arg) /* 106 */ {
    if(_debug) {
        cerr << "LOAD_ATTR | " << ((StringObject*)_frame->names()->get(arg))->value() << endl;
    }
    Object* v = pop();
    Object* w = _frame->names()->get(arg);
    Object* attr = v->getattr(w);
    /* load from klass_dict to stack */
    push(attr);
}

void Interpreter::compare_op(int arg) /* 107 */ {
    if(_debug) {
        cerr << "COMPARE_OP" << endl;
    }   
    Object *u, *v;
    u = pop();
    v = pop();
    switch (arg) {
    case LESS:
        push(v->less(u));
        break;
    case LESS_EQUAL:
        push(v->le(u));
        break;
    case EQUAL:
        push(v->eq(u));
        break;
    case NOT_EQUAL:
        push(v->ne(u));
        break;
    case GREATER_EQUAL:
        push(v->ge(u));
        break;
    case GREATER:
        push(v->greater(u));
        break;
    case IN:
        /* v in u */
        push(u->contains(v));
        break;
    case NOT_IN:
        if(u->contains(v) == Universe::True)
            push(Universe::False);
        else
            push(Universe::True);

        break;
    case IS:
        if(v == u)
            push(Universe::True);
        else
            push(Universe::False);
        break;
    case IS_NOT:
        if(v != u)
            push(Universe::True);
        else
            push(Universe::False);
        break;
    default:
        cout << "error: unrecognized compare op [" << arg << "]" << endl;
        exit(-1);
    }
}

/* 110 */

void Interpreter::jump_forward(int arg) /* 110 */{
    if(_debug) {
        cerr << "JUMP_FORWARD" << endl;
    }
    _frame->set_pc(_frame->get_pc() + arg);
}

void Interpreter::jump_absolute(int arg) /* 113 */ {
    if(_debug) {
        cerr << "JUMP_ABSOLUTE | " << arg << endl;
    }
    _frame->set_pc(arg);
}

void Interpreter::pop_jump_if_false(int arg) /* 114 */ {
    if(_debug) {
        cerr << "POP_JUMP_IF_FALSE" << endl;
    }
    Object* v = pop();
    if(v == Universe::False)
        _frame->set_pc(arg);
}

void Interpreter::load_global(int arg) /* 116 */ {
    if(_debug) {
        cerr << "LOAD_GLOBAL" << endl;
    }
    Object* name = _frame->names()->get(arg);
    Object* obj = _frame->globals()->get(name);
    if(obj != nullptr) {
        push(obj);
        return;
    }
    obj = _builtins->get(name);
    if(obj != nullptr) {
        push(obj);
        return;
    }
    push(Universe::None);

}

/* 120 */

void Interpreter::setup_loop(int arg) /* 120 */ {
    if(_debug) {
        cerr << "SETUP_LOOP" << endl;
    }
    _frame->loop_stack()->append(
        new LoopBlock(SETUP_LOOP, _frame->get_pc() + arg, stack_level())
    );
}

void Interpreter::load_fast(int arg) /* 124 */ {
    if(_debug) {
        cerr << "LOAD_FAST | " << arg << endl;
    }
    push(_frame->fast_locals()->get(arg));
}

void Interpreter::store_fast(int arg) /* 125 */ {
    if(_debug) {
        cerr << "STORE_FAST | " << arg << " | ";
    }
    int index = arg;
    Object* x = pop();
    if(_debug) {
        x->print();
        cout << endl;
    }
    _frame->fast_locals()->set(index, x);

}

/* 130 */

void Interpreter::call_function(int op_arg) /* 131 */{
    /* call function: build a frame with args and enter */
    if(_debug) {
        cerr << "CALL_FUNCTION";
    }
    ObjList* args = nullptr;
    if(op_arg > 0) {
        int nargs = op_arg & 0xFF;
        int nkwargs = op_arg >> 8;
        int nagiven = nargs + nkwargs * 2;
        if(_debug) {
            cout << "| given " << nagiven << " args, " << nkwargs << " of which are kwargs" << endl;
        }
        args = new ObjList(nagiven, equal2obj);
        while(nagiven--) {
            args->set(nagiven, pop());
        }
    }
    build_frame(pop(), args, op_arg);
    if(args != nullptr) {
        delete args;
    }
}

void Interpreter::make_function(int arg) /* 132 */ {
    if(_debug) {
        cerr << "MAKE_FUNCTION | default args = " << arg << endl;
    }
    Object* co = pop();
    FunctionObject* fo = new FunctionObject(co);

    fo->set_globals(_frame->globals());

    if(arg > 0) {
        ObjList* defaults = new ObjList(arg, equal2obj);
        /* default args */
        while(arg--) {
            defaults->set(arg, pop());
        }
        fo->set_defaults(defaults);
        delete defaults;
    }
    else
        fo->set_defaults((nullptr));

    push(fo);
}

void Interpreter::make_closure(int arg) /* 134 */ {
    if(_debug) {
        cerr << "MAKE_CLOSURE | default args = " << arg << endl;
    }
    Object* co = pop();
    FunctionObject* fo = new FunctionObject(co);
    fo->set_globals(_frame->globals());

    Object* closure_vars = pop();
    fo->set_closure((ListObject*)closure_vars);


    if(arg > 0) {
        ObjList* defaults = new ObjList(arg, equal2obj);
        /* default args */
        while(arg--) {
            defaults->set(arg, pop());
        }
        fo->set_defaults(defaults);
        delete defaults;
    }
    else
        fo->set_defaults((nullptr));

    push(fo);
}

void Interpreter::load_closure(int arg) /* 135 */ {
    if(_debug) {
        cerr << "LOAD_CLOSURE | " << arg << endl;
    }
    Object* v = _frame->closure()->get(arg);
    if(v == nullptr) {
        /* the variable is not local, but in parameter */
        v = _frame->get_cell_from_parameter(arg);
        _frame->closure()->set(arg, v);
    }
    if(v->klass() != CellKlass::get_instance())
        push(new CellObject(_frame->closure(), arg));
    else
        push(v);
}

void Interpreter::load_deref(int arg) /* 136 */ {
    if(_debug) {
        cerr << "LOAD_DEREF";
    }
    int index = arg;
    Object* deref= _frame->closure()->get(index);
    if(deref->klass() == CellKlass::get_instance()) 
        deref = ((CellObject*)deref)->value();
    push(deref);
    if(_debug) {
        cout << " | [" << arg << "] | " << deref->klass()->name() << " ";
        deref->print();
        cout << endl;
    }
}

void Interpreter::store_deref(int arg) /* 137 */ {
    if(_debug) {
        cerr << "STORE_DEREF";
    }
    int index = arg;
    Object* deref = pop();
    if(_debug) {
        cout << " | [" << arg << "] | " << deref->klass()->name() << " ";
        deref->print();
        cout << endl;
    }

    _frame->closure()->set(index, deref);
}
