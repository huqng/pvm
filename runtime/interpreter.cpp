
#include "../code/opcode.h"
#include "interpreter.h"
#include "../object/functionObject.h"
#include "universe.h"

#include <iostream>
using namespace std;

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

        //PInteger *lhs, *rhs;
        /* oprands */
        PObject *u, *v;
        PObject* retv;
        
        switch (op_code) {
        case POP_TOP: // 1
            pop();
            break;
        case BINARY_ADD: { // 23
            u = pop();
            v = pop();
            push(v->add(u));
            break;
        }
        case PRINT_ITEM: { // 71
            v = pop();
            v->print();
            break;
        }
        case PRINT_NEWLINE: { // 72
            cout << endl;
            break;
        }
        case BREAK_LOOP: { // 80
            LoopBlock* lb = _frame->loop_stack()->pop();
            while(stack_level() > lb->_level)
                pop();
            _frame->set_pc(lb->_target);
            break;
        }
        case RETURN_VALUE: // 83
            retv = pop();
            if(_frame->is_first_frame())
                return;
            leave_frame(retv);
            break;
        case POP_BLOCK: { // 87
                /* on normal end of loop */
                LoopBlock* lb = _frame->loop_stack()->pop();
                while(stack_level() > lb->_level)
                    pop();
                break;
            }
        case STORE_NAME: // 90
            /* 
             * varname = names[op_arg]
             * value = pop()
             * vars[varname] = value
             */
            _frame->locals()->put(_frame->names()->get(op_arg), pop());
            break;
        case LOAD_CONST: // 100
            push(_frame->consts()->get(op_arg));
            break;
        case LOAD_NAME: // 101
            /*
             * varname = names[op_arg]
             * value = vars[var_name]
             * push(value)
             */
            push(_frame->locals()->get(_frame->names()->get(op_arg)));

            break;
        case COMPARE_OP: // 107
            u = pop();
            v = pop();
            switch (op_arg) {
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
            break;
        case JUMP_FORWARD: // 110
            _frame->set_pc(_frame->get_pc() + op_arg);
            break;
        case JUMP_ABSOLUTE: // 113
            _frame->set_pc(op_arg);
            break;
        case POP_JUMP_IF_FALSE: // 114
            v = pop();
            if(v == Universe::PFalse)
                _frame->set_pc(op_arg);
            break;
        case SETUP_LOOP: // 120
            _frame->loop_stack()->add(
                new LoopBlock(op_code, _frame->get_pc() + op_arg, stack_level())
            );
            break;
        case CALL_FUNCTION: // 131
            build_frame(pop());
            break;
        case MAKE_FUNCTION: // 132
            v = pop();
            push(new FunctionObject(v));
            break;

        default:
            cout << "error: unrecognized opcode [" << (int)op_code << "]" << endl;
            exit(-1);
        }
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
