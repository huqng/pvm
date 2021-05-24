#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../code/codeObjet.h"
#include "../utils/map.h"

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
    /* initialized in run() */
    ArrayList<PObject*>* _stack;
    ArrayList<LoopBlock*>* _loop_stack;
    ArrayList<PObject*>* _consts;
    ArrayList<PObject*>* _names;

    Map<PString*, PObject*>* _vars;

    /* stack op */
    void        push(PObject* p);
    PObject*    pop();
    int         stack_level();
    
public:
    void run(CodeObject* co);
};

#endif
