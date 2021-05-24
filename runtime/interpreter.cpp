
#include "../code/opcode.h"
#include "interpreter.h"
#include "universe.h"

#include <iostream>
using namespace std;

void Interpreter::run(CodeObject* co) {
    int pc = 0;
    int code_length = co->_bytecodes->length();

    _stack = new ArrayList<PObject*>(co->_stacksize);
    _loop_stack = new ArrayList<LoopBlock*>();

    _consts = co->_consts;
    _names = co->_names;
    _vars = new Map<PString*, PObject*>();

    while(pc < code_length) {
        unsigned char op_code = co->_bytecodes->value()[pc++];
        bool has_arg = op_code >= HAVE_ARGUMENT;

        //cout << "opcode = " << (int)op_code << endl;

        int op_arg = -1;
        if(has_arg) {
            int byte1 = co->_bytecodes->value()[pc++] & 0xFF;
            int byte2 = co->_bytecodes->value()[pc++] & 0xFF;
            op_arg = byte2 << 8 | byte1;
        }

        //PInteger *lhs, *rhs;
        /* oprands */
        PObject *u, *v;
        
        switch (op_code) {
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
            LoopBlock* lb = _loop_stack->pop();
            while(stack_level() > lb->_level)
                pop();
            pc = lb->_target;
            break;
        }
        case RETURN_VALUE: // 83
            pop();
            break;
        case POP_BLOCK: { // 87
                /* on normal end of loop */
                LoopBlock* lb = _loop_stack->pop();
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
            _vars->put((PString*)_names->get(op_arg), pop());
            break;
        case LOAD_CONST: // 100
            push(_consts->get(op_arg));
            break;
        case LOAD_NAME: // 101
            /*
             * varname = names[op_arg]
             * value = vars[var_name]
             * push(value)
             */
            push(_vars->get((PString*)_names->get(op_arg)));

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
            pc += op_arg;
            break;
        case JUMP_ABSOLUTE: // 113
            pc = op_arg;
            break;
        case POP_JUMP_IF_FALSE: // 114
            v = pop();
            if(v == Universe::PFalse)
                pc = op_arg;
            break;
        case SETUP_LOOP: // 120
            _loop_stack->add(new LoopBlock(op_code, pc + op_arg, stack_level()));
            break;
        default:
            cout << "error: unrecognized opcode [" << (int)op_code << "]" << endl;
            exit(-1);
        }
    }

}

void Interpreter::push(PObject* p) {
    _stack->add(p);
}

PObject* Interpreter::pop() {
    return _stack->pop();
}

int Interpreter::stack_level() {
    return _stack->size();
}