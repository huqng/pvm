
#include "../code/opcode.h"
#include "interpreter.h"
#include "../object/functionObject.h"
#include "universe.h"

#include <iostream>
using namespace std;

Interpreter::Interpreter() {
    op = new op_t[256];
    for(int i = 0; i < 256; i++)
        op[i] = &Interpreter::unimplemented;
    
    op[1]   = &Interpreter::pop_top;
    op[23]  = &Interpreter::binary_add;
    op[71]  = &Interpreter::print_item;
    op[72]  = &Interpreter::print_newline;
    op[80]  = &Interpreter::break_loop;;
    op[83]  = &Interpreter::return_value;
    op[87]  = &Interpreter::pop_block;
    op[90]  = &Interpreter::store_name;
    op[97]  = &Interpreter::store_global;
    op[100] = &Interpreter::load_const;
    op[101] = &Interpreter::load_name;
    op[107] = &Interpreter::compare_op;
    op[110] = &Interpreter::jump_forward;
    op[113] = &Interpreter::jump_absolute;
    op[114] = &Interpreter::pop_jump_if_false;
    op[116] = &Interpreter::load_global;
    op[120] = &Interpreter::setup_loop;
    op[131] = &Interpreter::call_function;
    op[132] = &Interpreter::make_function;

    _frame = nullptr;
    _builtins = new Map<PObject*, PObject*>();
    _builtins->put(new StringObject("True"), Universe::PTrue);
    _builtins->put(new StringObject("False"), Universe::PFalse);
    _builtins->put(new StringObject("None"), Universe::PNone);

    debug = 0;
}

void Interpreter::run(CodeObject* co) {
    _frame = new FrameObject(co);
    eval_frame();
    destroy_frame();
}

void Interpreter::push(PObject* p) {
    _frame->stack()->add(p);
}

PObject* Interpreter::pop() {
    return _frame->stack()->pop();
}

int Interpreter::stack_level() {
    return _frame->stack()->size();
}

void Interpreter::build_frame(PObject* callable) {
    FrameObject* frame = new FrameObject((FunctionObject*)callable);
    frame->set_sender(_frame);
    _frame = frame;
}

void Interpreter::eval_frame() {    
    while(_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();

        //cout << "opcode = " << (int)op_code << endl;

        int op_arg = -1;
        bool has_arg = op_code >= HAVE_ARGUMENT;
        if(has_arg) {
            op_arg = _frame->get_op_arg();
        }

        if(op[op_code] == &Interpreter::unimplemented) {
            cout << "Unimplemented opcode [" << op_code << "]" << endl;
        }
        (this->*op[op_code])(op_arg);
    }
}

void Interpreter::leave_frame(PObject* retv) {
    destroy_frame();
    push(retv);
}

void Interpreter::destroy_frame() {
    FrameObject* tmp = _frame;
    _frame = _frame->sender();
    delete tmp;
}

/* instructions */

void Interpreter::unimplemented(int arg) { 
    cout << "error: unrecognized opcode" << endl;
    exit(-1);
}

void Interpreter::pop_top(int arg) {
    pop();
    if(debug) {
        cerr << "POP_TOP" << endl;
    }
}

void Interpreter::binary_add(int arg) {
    PObject *u, *v;
    u = pop();
    v = pop();
    push(v->add(u));
    if(debug) {
        cerr << "BINARY_ADD" << endl;
    }
}

void Interpreter::print_item(int arg) {
    PObject* u = pop();
    u->print();
    if(debug) {
        cerr << "PRINT_ITEM" << endl;
    }
}

void Interpreter::print_newline(int arg) {
    cout << endl;
    if(debug) {
        cerr << "PRINT_NEWLINE" << endl;
    }
}

void Interpreter::break_loop(int arg) {
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    _frame->set_pc(lb->_target);
    if(debug) {
        cerr << "BREAK_LOOP" << endl;
    }
}

void Interpreter::return_value(int arg) {
    PObject* retv = pop();
    if(_frame->is_first_frame())
        return;
    leave_frame(retv);
    if(debug) {
        cerr << "RETURN_VALUE" << endl;
    }
}

void Interpreter::pop_block(int arg) {
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    if(debug) {
        cerr << "POP_BLOCK" << endl;
    }
}

void Interpreter::store_name(int arg) {
    _frame->locals()->put(_frame->names()->get(arg), pop());
    if(debug) {
        cerr << "STORE_NAME" << endl;
    }
}

void Interpreter::store_global(int arg) {
    _frame->globals()->put(_frame->names()->get(arg), pop());
    if(debug) {
        cerr << "STORE_GLOBAL" << endl;
    }
}

void Interpreter::load_const(int arg) {
    push(_frame->consts()->get(arg));
    if(debug) {
        cerr << "LOAD_CONST" << endl;
    }
}

void Interpreter::load_name(int arg) {
    PObject* name = _frame->names()->get(arg);
    PObject* obj = _frame->locals()->get(name);
    if(obj != Universe::PNone) {
        push(obj);
        return;
    }
    obj = _frame->globals()->get(name);
    if(obj != Universe::PNone) {
        push(obj);
        return;
    }
    obj = _builtins->get(name);
    if(obj != Universe::PNone) {
        push(obj);
        return;
    }
    push(Universe::PNone);

    if(debug) {
        cerr << "LOAD_NAME" << endl;
    }
}

void Interpreter::compare_op(int arg) {
    PObject *u, *v;
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
    case IS:
        if(v == u)
            push(Universe::PTrue);
        else
            push(Universe::PFalse);
        break;
    case IS_NOT:
        if(v != u)
            push(Universe::PTrue);
        else
            push(Universe::PFalse);
        break;
    default:
        cout << "error: unrecognized compare op [" << arg << "]" << endl;
        exit(-1);
    }
    if(debug) {
        cerr << "COMPARE_OP" << endl;
    }   
}

void Interpreter::jump_forward(int arg) {
    _frame->set_pc(_frame->get_pc() + arg);
    if(debug) {
        cerr << "JUMP_FORWARD" << endl;
    }
}

void Interpreter::jump_absolute(int arg) {
    _frame->set_pc(arg);
    if(debug) {
        cerr << "JUMP_ABSOLUTE" << endl;
    }
}

void Interpreter::pop_jump_if_false(int arg) {
    PObject* v = pop();
    if(v == Universe::PFalse)
        _frame->set_pc(arg);
    if(debug) {
        cerr << "POP_JUMP_IF_FALSE" << endl;
    }
}

void Interpreter::load_global(int arg) {
    PObject* name = _frame->names()->get(arg);
    PObject* obj = _frame->globals()->get(name);
    if(obj != Universe::PNone) {
        push(obj);
        return;
    }
    push(Universe::PNone);

    if(debug) {
        cerr << "LOAD_GLOBAL" << endl;
    }
}

void Interpreter::setup_loop(int arg) {
    _frame->loop_stack()->add(
        new LoopBlock(SETUP_LOOP, _frame->get_pc() + arg, stack_level())
    );
    if(debug) {
        cerr << "SETUP_LOOP" << endl;
    }
}

void Interpreter::call_function(int arg) {
    build_frame(pop());
    if(debug) {
        cerr << "CALL_FUNCTION" << endl;
    }
}

void Interpreter::make_function(int arg) {
    PObject* v = pop();
    FunctionObject* fo = new FunctionObject(v);
    fo->set_globals(_frame->globals());
    push(fo);
    if(debug) {
        cerr << "MAKE_FUNCTION" << endl;
    }
}
