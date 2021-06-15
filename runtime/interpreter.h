#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../utils/map.h"
#include "frame.h"

class CodeObject;

class LoopBlock;
class Interpreter;

typedef void(Interpreter::*op_t)(int);

class LoopBlock {
public:
    unsigned char _type;
    unsigned int _target;
    int _level;
    LoopBlock(
        unsigned char b_type, 
        unsigned int b_target, 
        int b_level):
            _type(b_type),
            _target(b_target),
            _level(b_level) {

        }
};

class Interpreter {
private:
    Map<PObject*, PObject*>* _builtins;
    Frame* _frame;
    bool        debug;

    /* stack op */
    void        push(PObject* p);
    PObject*    pop();
    int         stack_level();
    
    void        build_frame(PObject* callable, ArrayList<PObject*>* args);
    void        eval_frame();
    void        leave_frame(PObject* retv);
    void        destroy_frame();

    /* instructions */
    op_t*   op;

    void    unimplemented(int);         // ?
    void    pop_top (int);              // 1
    void    binary_add(int);            // 23
    void    print_item(int);            // 71
    void    print_newline(int);         // 72
    void    break_loop(int);            // 80
    void    return_value(int);          // 83
    void    pop_block(int);             // 87
    void    store_name(int);            // 90
    void    store_global(int);          // 97
    void    load_const(int);            // 100
    void    load_name(int);             // 101
    void    compare_op(int);            // 107
    void    jump_forward(int);          // 110
    void    jump_absolute(int);         // 113
    void    pop_jump_if_false(int);     // 114
    void    load_global(int);           // 116 
    void    setup_loop(int);            // 120
    void    load_fast(int);             // 124
    void    call_function(int);         // 131
    void    make_function(int);         // 132

public:
    Interpreter();
    void run(CodeObject* co);
};

#endif
