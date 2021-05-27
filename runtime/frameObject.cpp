#include "frameObject.h"

FrameObject::FrameObject(CodeObject* co) {
    _stack = new ArrayList<PObject*>();
    _loop_stack = new ArrayList<LoopBlock*>();

    _consts = co->_consts;
    _names = co->_names;
    _locals = new Map<PObject*, PObject*>(obj_eq);
    _globals = _locals;
    _fast_locals = nullptr;

    _co = co;
    _pc = 0;
    _sender = nullptr;
}

FrameObject::FrameObject(FunctionObject* fo, ObjList* args) {
    _stack = new ObjList();
    _loop_stack = new ArrayList<LoopBlock*>();

    _co = fo->_func_code;

    _consts = _co->_consts;
    _names = _co->_names;
    _locals = new Map<PObject*, PObject*>(obj_eq);
    _globals = fo->globals();

    if(_co->_argcount > 0) {
        if(args->size() + fo->_defaults->size() < _co->_argcount) {
            cerr << "FrameObject: too few arguments" << endl;
            exit(-1);
        }
        _fast_locals = new ObjList(_co->_argcount);
        int dft_cnt = fo->_defaults->length();
        int argc_nt = _co->_argcount;
        while(dft_cnt--) {
            _fast_locals->set(--argc_nt, fo->_defaults->get(dft_cnt));
        }

        for(int i = 0; i < args->length(); i++) {
            _fast_locals->set(i, args->get(i));
        }
    }
    else
        _fast_locals = nullptr;

    _pc = 0;
    _sender = nullptr;
}

FrameObject::FrameObject() {

}

bool FrameObject::has_more_codes() {
    return _pc < _co->_bytecodes->length();
}

unsigned char FrameObject::get_op_code() {
    return _co->_bytecodes->value()[_pc++];
}

int FrameObject::get_op_arg() {
    int byte1 = _co->_bytecodes->value()[_pc++] & 0xFF;
    int byte2 = _co->_bytecodes->value()[_pc++] & 0xFF;
    return byte2 << 8 | byte1;
}
