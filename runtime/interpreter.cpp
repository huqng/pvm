#include "interpreter.h"

/* String Table */

StringTable* StringTable::instance = nullptr;

StringTable::StringTable() {
    str_next = new StringObject("next");
}

StringTable* StringTable::get_instance() {
    if(instance == nullptr)
        instance = new StringTable();
    return instance;
}

/* Interpreter */

Interpreter::Interpreter(int debug) {
    this->dbg = debug;

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
    op[80]  = &Interpreter::break_loop;;
    op[83]  = &Interpreter::return_value;
    op[87]  = &Interpreter::pop_block;
    op[90]  = &Interpreter::store_name;
    op[92]  = &Interpreter::unpack_sequence;
    op[93]  = &Interpreter::for_iter;
    op[97]  = &Interpreter::store_global;
    op[100] = &Interpreter::load_const;
    op[101] = &Interpreter::load_name;
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
    op[131] = &Interpreter::call_function;
    op[132] = &Interpreter::make_function;

    _frame = nullptr; /* initialize from codeObject when run() */
    _builtins = new DictObject();
    _builtins->put(new StringObject("True"), Universe::True);
    _builtins->put(new StringObject("False"), Universe::False);
    _builtins->put(new StringObject("None"), Universe::None);
    _builtins->put(new StringObject("len"), new FunctionObject(len));
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
        if(dbg) {
            cerr << "\t<Native Function>" << endl;
        }
        push(((FunctionObject*)callable)->call(args));
    }
    else if(callable->klass() == FunctionKlass::get_instance()) {
        if(dbg) {
            cerr << "\t<Non-Native Function>" << endl;
        }
        Frame* frame = new Frame((FunctionObject*)callable, args, oparg);
        frame->set_sender(_frame);
        _frame = frame;
    }
    else if(callable->klass() == MethodKlass::get_instance()) {
        if(dbg) {
            cerr << "\t<Method>";
        }
        MethodObject* method = (MethodObject*)callable;
        if(args == nullptr) {
            args = new ObjList();
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, oparg);
    }
    else {
        cerr << "Error build_frame, unrecognized klass = " << callable->klass() << endl;
        cerr << "\tinteger klass = " << IntegerKlass::get_instance() << endl;
        cerr << "\tstring klass = " << StringKlass::get_instance() << endl;
        cerr << "\tfunction klass = " << FunctionKlass::get_instance() << endl;
        cerr << "\tnative function klass = " << NativeFunctionKlass::get_instance() << endl;
        cerr << "\tmethod klass = " << MethodKlass::get_instance() << endl;
    }
}

void Interpreter::eval_frame() {    
    while(_frame->has_more_codes()) {

        /* pc is updated automatically in get_op_code() or get_op_arg() */
        unsigned char op_code = _frame->get_op_code();

        if(dbg)
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
    if(dbg) {
        cerr << "POP_TOP" << endl;
    }
    pop();
}

/* 10 */

/* 20 */

void Interpreter::binary_multiply(int arg) /* 20*/ {
    if(dbg) {
        cerr << "BINARY_MULTIPLY" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->mul(u));
}

void Interpreter::binary_divide(int arg) /* 21 */ {
    if(dbg) {
        cerr << "BINARY_DIVIDE" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->div(u));
}

void Interpreter::binary_modulo(int arg) /* 22 */ {
    if(dbg) {
        cerr << "BINARY_MODULO" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->mod(u));
}

void Interpreter::binary_add(int arg) /* 23 */ {
    if(dbg) {
        cerr << "BINARY_ADD" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->add(u));
}

void Interpreter::binary_subtract(int arg) /* 24 */ {
    if(dbg) {
        cerr << "BINARY_SUBTRACT" << endl;
    }
    Object *u, *v;
    u = pop();
    v = pop();
    push(v->sub(u));
}

void Interpreter::binary_subscr(int arg) /* 25 */ {
    if(dbg) {
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
    if(dbg) {
        cerr << "STORE_MAP" << endl;
    }
    Object* key = pop();
    Object* value = pop();
    DictObject* dict = (DictObject*)top();
    dict->put(key, value);
}

/* 60 */

void Interpreter::store_subscr(int arg) /* 60 */ {
    if(dbg) {
        cerr << "STORE_SUBSCR" << endl;
    }
    Object* x = pop();
    Object* obj = pop();
    Object* index = pop();
    obj->store_subscr(x, index);
}

void Interpreter::delete_subscr(int arg) /* 61 */ {
    if(dbg) {
        cerr << "DELETE_SUBSCR";
    }
    Object* index = pop();
    Object* obj = pop();
    obj->del_subscr(index);
}

void Interpreter::get_iter(int arg) /* 68 */ {
    if(dbg) {
        cerr << "GET_ITER" << endl;
    }
    Object* obj = pop();
    push(obj->iter());
}

/* 70 */

void Interpreter::print_item(int arg) /* 71 */ {
    if(dbg) {
        cerr << "PRINT_ITEM | ";
    }
    Object* obj = pop();
    obj->print();
    if(dbg) {
        cerr << endl;
    }
}

void Interpreter::print_newline(int arg) /* 72 */ {
    if(dbg) {
        cerr << "PRINT_NEWLINE" << endl;
    }
    cout << endl;
}

/* 80 */

void Interpreter::break_loop(int arg) /* 80 */ {
    if(dbg) {
        cerr << "BREAK_LOOP" << endl;
    }
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    _frame->set_pc(lb->_target);
}

void Interpreter::return_value(int arg) /* 83 */ {
    if(dbg) {
        cerr << "RETURN_VALUE" << endl;
    }
    Object* retv = pop();
    if(_frame->is_first_frame())
        return;
    leave_frame(retv);
}

void Interpreter::pop_block(int arg) /* 87 */ {
    if(dbg) {
        cerr << "POP_BLOCK" << endl;
    }
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
}

/* 90 */

void Interpreter::store_name(int arg) /* 90 */ {
    if(dbg) {
        cerr << "STORE_NAME | " << ((StringObject*)_frame->names()->get(arg))->value() << endl;
    }
    _frame->locals()->put(_frame->names()->get(arg), pop());
}

void Interpreter::unpack_sequence(int arg) /* 92 */ {
    if(dbg) {
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
    if(dbg) {
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
    if(dbg) {
        cerr << "STORE_GLOBAL" << endl;
    }
    _frame->globals()->put(_frame->names()->get(arg), pop());
}

/* 100 */

void Interpreter::load_const(int arg) /* 100 */ {
    if(dbg) {
        cerr << "LOAD_CONST | [" << arg << "] | ";
    }
    Object* v = _frame->consts()->get(arg);
    if(v == nullptr)
        v = Universe::None;
    if(dbg) {
        cout << v->klass()->name() << " ";
        v->print();
        cout << endl;
    }
    push(v);
}

void Interpreter::load_name(int arg) /* 101 */ {
    if(dbg) {
        cerr << "LOAD_NAME | " << ((StringObject*)_frame->names()->get(arg))->value() << endl;
    }
    Object* name = _frame->names()->get(arg);
    Object* obj = _frame->locals()->get(name);
    if(obj != nullptr) {
        push(obj);
        return;
    }
    obj = _frame->globals()->get(name);
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

void Interpreter::build_list(int arg) { /* 103 */
    if(dbg) {
        cerr << "BUILD_LIST | [" << arg << "]" << endl;
    }
    Object* lst = new ListObject();
    while(arg--) {
        ((ListObject*)lst)->set(arg, pop());
    }
    push(lst);
}

void Interpreter::build_map(int arg) /* 105 */ {
    if(dbg) {
        cerr << "BUILD_MAP" << endl;
    }
    Object* dict = new DictObject();
    push(dict);    
}

void Interpreter::load_attr(int arg) {
    if(dbg) {
        cerr << "LOAD_ATTR | " << ((StringObject*)_frame->names()->get(arg))->value() << endl;
    }
    Object* v = pop();
    Object* w = _frame->names()->get(arg);
    Object* attr = v->getattr(w);
    /* load from klass_dict to stack */
    push(attr);
}

void Interpreter::compare_op(int arg) {
    if(dbg) {
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

void Interpreter::jump_forward(int arg) {
    if(dbg) {
        cerr << "JUMP_FORWARD" << endl;
    }
    _frame->set_pc(_frame->get_pc() + arg);
}

void Interpreter::jump_absolute(int arg) /* 113 */ {
    if(dbg) {
        cerr << "JUMP_ABSOLUTE | " << arg << endl;
    }
    _frame->set_pc(arg);
}

void Interpreter::pop_jump_if_false(int arg) {
    if(dbg) {
        cerr << "POP_JUMP_IF_FALSE" << endl;
    }
    Object* v = pop();
    if(v == Universe::False)
        _frame->set_pc(arg);
}

void Interpreter::load_global(int arg) {
    if(dbg) {
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

void Interpreter::setup_loop(int arg) {
    if(dbg) {
        cerr << "SETUP_LOOP" << endl;
    }
    _frame->loop_stack()->append(
        new LoopBlock(SETUP_LOOP, _frame->get_pc() + arg, stack_level())
    );
}

void Interpreter::load_fast(int arg) {
    if(dbg) {
        cerr << "LOAD_FAST | " << arg << endl;
    }
    push(_frame->fast_locals()->get(arg));
}

/* 130 */

void Interpreter::call_function(int op_arg) {
    /* call function: build a frame with args and enter */
    if(dbg) {
        cerr << "CALL_FUNCTION";
    }
    ArrayList<Object*>* args = nullptr;
    if(op_arg > 0) {
        int nargs = op_arg & 0xFF;
        int nkwargs = op_arg >> 8;
        int nagiven = nargs + nkwargs * 2;
        if(dbg) {
            cout << "| given " << nagiven << " args, " << nkwargs << " of which are kwargs" << endl;
        }
        args = new ObjList(nagiven);
        while(nagiven--) {
            args->set(nagiven, pop());
        }
    }
    build_frame(pop(), args, op_arg);
    if(args != nullptr) {
        delete args;
    }
}

void Interpreter::make_function(int arg) {
    if(dbg) {
        cerr << "MAKE_FUNCTION | default args = " << arg << endl;
    }
    Object* v = pop();
    FunctionObject* fo = new FunctionObject(v);

    fo->set_globals(_frame->globals());

    if(arg > 0) {
        /* default args */
        ArrayList<Object*>* defaults = new ArrayList<Object*>(arg);
        while(arg--) {
            defaults->set(arg, pop());
        }
        fo->set_defaults(defaults);
        delete defaults;
    }
    else
        fo->set_defaults(new ArrayList<Object*>(arg));

    push(fo);
}
