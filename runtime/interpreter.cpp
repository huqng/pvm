#include "../code/opcode.h"
#include "../object/pInteger.h"
#include "interpreter.h"
#include "universe.h"

#include <iostream>
using namespace std;

void Interpreter::run(CodeObject* co) {
    int pc = 0;
    int code_length = co->_bytecodes->length();

    _stack = new ArrayList<PObject*>(co->_stacksize);
    _consts = co->_consts;
    _names = co->_names;
    _vars = new Map<PString*, PObject*>();

    while(pc < code_length) {
        unsigned char op_code = co->_bytecodes->value()[pc++];
        bool has_arg = op_code >= HAVE_ARGUMENT;
        
        int op_arg = -1;
        if(has_arg) {
            int byte1 = co->_bytecodes->value()[pc++] & 0xFF;
            int byte2 = co->_bytecodes->value()[pc++] & 0xFF;
            op_arg = byte2 << 8 | byte1;
        }

        PInteger *lhs, *rhs;
        PObject *u, *v, *w, *attr;
        
        switch (op_code) {
        case BINARY_ADD: // 23
            v = pop();
            w = pop();
            push(w->add(v));
            break;
        case PRINT_ITEM: // 71
            v = pop();
            v->print();
            break;
        case PRINT_NEWLINE: // 72
            cout << endl;
            break;
        case RETURN_VALUE: // 83
            pop();
            break;
        case STORE_NAME: // 90
            /* add a variable named _names[oparg] 
             * whose value is on the top of stack
             * get value by pop
             * add to vars table
             */
            _vars->put((PString*)_names->get(op_arg), pop());
            break;
        case LOAD_CONST: // 100
            push(_consts->get(op_arg));
            break;
        case LOAD_NAME: // 101
            push(_vars->get((PString*)_names->get(op_arg)));

            break;
        case COMPARE_OP: // 107
            w = pop();
            v = pop();
            switch (op_arg) {
            case LESS:
                push(v->less(w));
                break;
            case LESS_EQUAL:
                push(v->le(w));
                break;
            case EQUAL:
                push(v->eq(w));
                break;
            case NOT_EQUAL:
                push(v->ne(w));
                break;
            case GREATER_EQUAL:
                push(v->ge(w));
                break;
            case GREATER:
                push(v->greater(w));
                break;
            default:
                break;
            }
            break;
        case JUMP_FORWARD: // 110
            pc += op_arg;
            break;
        case POP_JUMP_IF_FALSE: // 114
            v = pop();
            if(v == Universe::PFalse)
                pc = op_arg;
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
