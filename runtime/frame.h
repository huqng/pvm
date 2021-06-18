#ifndef _FRAME_H
#define _FRAME_H

#include "functionObject.h"
#include "map.h"

class LoopBlock;

class Frame {
private:

    ObjList*    _stack;
    ArrayList<LoopBlock*>*  _loop_stack;

    ObjList*    _consts;
    ObjList*    _names;

    Map<PObject*, PObject*>*    _locals;
    Map<PObject*, PObject*>*    _globals;
    ObjList*    _fast_locals;

    CodeObject*     _co;
    int             _pc;

    Frame*    _sender;
public:
    /* can built from functionObject or CodeObject (entry)*/
    Frame(FunctionObject* fo, ArrayList<PObject*>* args);
    Frame(CodeObject* co);
    Frame();
    
    void    set_pc(int x) { _pc = x; }
    int     get_pc() { return _pc; }

    ObjList*    stack() {
        return _stack;
    }
    ArrayList<LoopBlock*>*  loop_stack() {
        return _loop_stack;
    }

    ObjList*    consts() {
        return _consts;
    }
    ObjList*    names() {
        return _names;
    }
    Map<PObject*, PObject*>*    locals() {
        return _locals;
    }
    Map<PObject*, PObject*>*    globals() {
        return _globals;
    }
    ObjList*    fast_locals() {
        return _fast_locals;
    }

    bool            has_more_codes();
    unsigned char   get_op_code();
    int             get_op_arg();

    void set_sender(Frame* x) {
        _sender = x;
    }
    
    Frame* sender() {
        return _sender;
    }

    bool is_first_frame() {
        return _sender == nullptr;
    }
};

#endif
