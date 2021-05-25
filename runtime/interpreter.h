#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../utils/map.h"
#include "frameObject.h"

class CodeObject;

class LoopBlock;
class Interpreter;

typedef void(Interpreter::*op_t)(int, int);

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
    FrameObject* _frame;
    bool        debug;

    /* stack op */
    void        push(PObject* p);
    PObject*    pop();
    int         stack_level();
    
    void        build_frame(PObject* callable);
    void        eval_frame();
    void        leave_frame(PObject* retv);
    void        destroy_frame();

    /* instructions */
    op_t*   op;

    void    unimplemented(int, int);     // ?
    void    pop_top (int, int);          // 1
    void    binary_add(int, int);        // 23
    void    print_item(int, int);        // 71
    void    print_newline(int, int);     // 72
    void    break_loop(int, int);        // 80
    void    return_value(int, int);      // 83
    void    pop_block(int, int);         // 87
    void    store_name(int, int);        // 90
    void    load_const(int, int);        // 100
    void    load_name(int, int);         // 101
    void    compare_op(int, int);        // 107
    void    jump_forward(int, int);      // 110
    void    jump_absolute(int, int);     // 113
    void    pop_jump_if_false(int, int); // 114
    void    setup_loop(int, int);        // 120
    void    call_function(int, int);     // 131
    void    make_function(int, int);     // 132

public:
    Interpreter();
    void run(CodeObject* co);
};

#endif
