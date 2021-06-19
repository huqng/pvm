#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "map.h"
#include "frame.h"

class CodeObject;

class LoopBlock;
class Interpreter;
class StringTable;

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
    Map<Object*, Object*>* _builtins;
    Frame* _frame;
    bool        debug;

    /* stack op */
    void    push(Object* p);
    Object* pop();
    Object* top();
    int     stack_level();
    
    void    build_frame(Object* callable, ArrayList<Object*>* args);
    void    eval_frame();
    void    leave_frame(Object* retv);
    void    destroy_frame();

    /* instructions */
    op_t*   op;

    void    unimplemented(int);         // ?
    void    pop_top (int);              // 1
    void    binary_multiply(int);       // 20
    void    binary_divide(int);         // 21
    void    binary_modulo(int);         // 22
    void    binary_add(int);            // 23
    void    binary_subtract(int);       // 24
    void    binary_subscr(int);         // 25
    void    store_subscr(int);          // 60
    void    delete_subscr(int);         // 61
    void    get_iter(int);              // 58
    void    print_item(int);            // 71
    void    print_newline(int);         // 72
    void    break_loop(int);            // 80
    void    return_value(int);          // 83
    void    pop_block(int);             // 87
    void    store_name(int);            // 90
    void    for_iter(int);              // 93
    void    store_global(int);          // 97
    void    load_const(int);            // 100
    void    load_name(int);             // 101
    void    build_list(int);            // 103
    void    load_attr(int);             // 106
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

class StringTable {
private:
    static StringTable* instance;
    StringTable();
public:
    static StringTable* get_instance();

    StringObject* str_next;
};

#endif
