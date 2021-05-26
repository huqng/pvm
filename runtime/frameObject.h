#ifndef _FRAME_OBJECT_H
#define _FRAME_OBJECT_H

#include "../object/functionObject.h"
#include "../utils/map.h"

class LoopBlock;

class FrameObject {
private:

    ArrayList<PObject*>*    _stack;
    ArrayList<LoopBlock*>*  _loop_stack;

    ArrayList<PObject*>*    _consts;
    ArrayList<PObject*>*    _names;

    Map<PObject*, PObject*>*    _locals;
    Map<PObject*, PObject*>*    _globals;
    ArrayList<PObject*>*        _fast_locals;

    CodeObject*             _co;
    int                     _pc;

    FrameObject*            _sender;
public:
    FrameObject(FunctionObject* fo, ArrayList<PObject*>* args);
    FrameObject(CodeObject* co);
    FrameObject();
    
    void    set_pc(int x) { _pc = x; }
    int     get_pc() { return _pc; }

    ArrayList<PObject*>*    stack() {
        return _stack;
    }
    ArrayList<LoopBlock*>*  loop_stack() {
        return _loop_stack;
    }

    ArrayList<PObject*>*    consts() {
        return _consts;
    }
    ArrayList<PObject*>*    names() {
        return _names;
    }
    Map<PObject*, PObject*>*    locals() {
        return _locals;
    }
    Map<PObject*, PObject*>*    globals() {
        return _globals;
    }
    ArrayList<PObject*>*    fast_locals() {
        return _fast_locals;
    }

    bool            has_more_codes();
    unsigned char   get_op_code();
    int             get_op_arg();

    void set_sender(FrameObject* x) {
        _sender = x;
    }
    
    FrameObject* sender() {
        return _sender;
    }

    bool is_first_frame() {
        return _sender == nullptr;
    }
};

#endif
