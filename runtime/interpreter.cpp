
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
    op[100] = &Interpreter::load_const;
    op[101] = &Interpreter::load_name;
    op[107] = &Interpreter::compare_op;
    op[110] = &Interpreter::jump_forward;
    op[113] = &Interpreter::jump_absolute;
    op[114] = &Interpreter::pop_jump_if_false;
    op[120] = &Interpreter::setup_loop;
    op[131] = &Interpreter::call_function;
    op[132] = &Interpreter::make_function;

    _frame = nullptr;
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
        bool has_arg = op_code >= HAVE_ARGUMENT;

        //cout << "opcode = " << (int)op_code << endl;

        int op_arg = -1;
        if(has_arg) {
            op_arg = _frame->get_op_arg();
        }

        /* oprands */
        PObject *u, *v;
        PObject* retv;

        (this->*op[op_code])(op_code, op_arg);
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

void Interpreter::unimplemented(int code, int arg) { 
    cout << "error: unrecognized opcode [" << code << "]" << endl;
    exit(-1);
}

void Interpreter::pop_top(int code, int arg) {
    pop();
    if(debug) {
        cerr << "POP_TOP" << endl;
    }
}

void Interpreter::binary_add(int code, int arg) {
    PObject *u, *v;
    u = pop();
    v = pop();
    push(v->add(u));
    if(debug) {
        cerr << "BINARY_ADD" << endl;
    }
}

void Interpreter::print_item(int code, int arg) {
    PObject* u = pop();
    u->print();
    if(debug) {
        cerr << "PRINT_ITEM (omitted)" << endl;
    }
}

void Interpreter::print_newline(int code, int arg) {
    cout << endl;
    if(debug) {
        cerr << "PRINT_NEWLINE" << endl;
    }
}

void Interpreter::break_loop(int code, int arg) {
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    _frame->set_pc(lb->_target);
    if(debug) {
        cerr << "BREAK_LOOP" << endl;
    }
}

void Interpreter::return_value(int code, int arg) {
    PObject* retv = pop();
    if(_frame->is_first_frame())
        return;
    leave_frame(retv);
    if(debug) {
        cerr << "RETURN_VALUE";
    }
}

void Interpreter::pop_block(int code, int arg) {
    LoopBlock* lb = _frame->loop_stack()->pop();
    while(stack_level() > lb->_level)
        pop();
    if(debug) {
        cerr << "POP_BLOCK";
    }
}

void Interpreter::store_name(int code, int arg) {
    _frame->locals()->put(_frame->names()->get(arg), pop());
    if(debug) {
        cerr << "STORE_NAME" << endl;
    }
}

void Interpreter::load_const(int code, int arg) {
    push(_frame->consts()->get(arg));
    if(debug) {
        cerr << "LOAD_CONST" << endl;
    }
}

void Interpreter::load_name(int code, int arg) {
    push(_frame->locals()->get(_frame->names()->get(arg)));
    if(debug) {
        cerr << "LOAD_NAME" << endl;
    }
}

void Interpreter::compare_op(int code, int arg) {
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
    default:
        break;
    }
    if(debug) {
        cerr << "COMPARE_OP" << endl;
    }   
}

void Interpreter::jump_forward(int code, int arg) {
    _frame->set_pc(_frame->get_pc() + arg);
    if(debug) {
        cerr << "JUMP_FORWARD" << endl;
    }
}

void Interpreter::jump_absolute(int code, int arg) {
    _frame->set_pc(arg);
    if(debug) {
        cerr << "JUMP_ABSOLUTE" << endl;
    }
}

void Interpreter::pop_jump_if_false(int code, int arg) {
    PObject* v = pop();
    if(v == Universe::PFalse)
        _frame->set_pc(arg);
    if(debug) {
        cerr << "POP_JUMP_IF_FALSE" << endl;
    }
}

void Interpreter::setup_loop(int code, int arg) {
    _frame->loop_stack()->add(
        new LoopBlock(code, _frame->get_pc() + arg, stack_level())
    );
    if(debug) {
        cerr << "SETUP_LOOP" << endl;
    }
}

void Interpreter::call_function(int code, int arg) {
    build_frame(pop());
    if(debug) {
        cerr << "CALL_FUNCTION" << endl;
    }
}

void Interpreter::make_function(int code, int arg) {
    PObject* v = pop();
    push(new FunctionObject(v));
    if(debug) {
        cerr << "MAKE_FUNCTION" << endl;
    }
}
