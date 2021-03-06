#ifndef _FRAME_H
#define _FRAME_H

#include "arrayList.h"

class ListObject;
class DictObject;
class CodeObject;
class FunctionObject;
class OopClosure;
class LoopBlock;

class Frame {
private:
    bool _entry_frame;

    ListObject*    _stack;
    ArrayList<LoopBlock*>*  _loop_stack;

    ListObject*    _consts;
    ListObject*    _names;

    DictObject* _locals;
    DictObject* _globals;
    ListObject* _fast_locals;
    ListObject* _closure; 

    CodeObject* _co;
    int         _pc;

    Frame*    _sender;
public:
    /* can built from functionObject or CodeObject (entry)*/
    Frame(FunctionObject* fo, ObjList* args, int op_arg);
    Frame(CodeObject* co);
    Frame();
    ~Frame();
    
    void    set_pc(int x) { _pc = x; }
    int     get_pc() { return _pc; }
    Object* get_cell_from_parameter(int index);

    ListObject* stack() {
        return _stack;
    }
    ArrayList<LoopBlock*>*  loop_stack() {
        return _loop_stack;
    }

    ListObject* consts() {
        return _consts;
    }
    ListObject* names() {
        return _names;
    }
    DictObject* locals() {
        return _locals;
    }
    DictObject* globals() {
        return _globals;
    }
    ListObject* fast_locals() {
        return _fast_locals;
    }
    ListObject* closure() {
        return _closure;
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

    void set_entry_frame(bool b) {
        _entry_frame = b;
    }

    bool is_entry_frame() {
        return _entry_frame;
    }

    void oops_do(OopClosure* closure);

};

#endif
