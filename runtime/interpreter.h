#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../utils/map.h"
#include "frameObject.h"

class CodeObject;

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

    /* stack op */
    void        push(PObject* p);
    PObject*    pop();
    int         stack_level();
    
    void        build_frame(PObject* callable);
    void        eval_frame();
    void        leave_frame(PObject* retv);
    void        destroy_frame();
public:
    void run(CodeObject* co);
};

#endif
