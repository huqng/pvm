#include "frame.h"

Frame::Frame(CodeObject* co) {
    _stack = new ObjList();
    _loop_stack = new ArrayList<LoopBlock*>();

    _consts = co->_consts;
    _names = co->_names;
    _locals = new Map<Object*, Object*>(equal2obj);
    _globals = _locals;
    _fast_locals = nullptr;

    _co = co;
    _pc = 0;
    _sender = nullptr;
}

Frame::Frame(FunctionObject* fo, ObjList* args) {
    _stack = new ObjList();
    _loop_stack = new ArrayList<LoopBlock*>();

    _co = fo->_func_code;

    _consts = _co->_consts;
    _names = _co->_names;
    _locals = new ObjDict(equal2obj);
    _globals = fo->globals();

    if(_co->_argcount > 0) {
        if(args->size() + fo->_defaults->size() < _co->_argcount) {
            cerr << "Frame: too few arguments" << endl;
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

Frame::Frame() {

}

bool Frame::has_more_codes() {
    return _pc < _co->_bytecodes->length();
}

unsigned char Frame::get_op_code() {
    return _co->_bytecodes->value()[_pc++];
}

int Frame::get_op_arg() {
    int byte1 = _co->_bytecodes->value()[_pc++] & 0xFF;
    int byte2 = _co->_bytecodes->value()[_pc++] & 0xFF;
    return byte2 << 8 | byte1;
}
