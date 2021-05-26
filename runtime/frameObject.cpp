#include "frameObject.h"

FrameObject::FrameObject(CodeObject* co) {
    _stack = new ArrayList<PObject*>();
    _loop_stack = new ArrayList<LoopBlock*>();

    _consts = co->_consts;
    _names = co->_names;
    _locals = new Map<PObject*, PObject*>();
    _globals = _locals;

    _co = co;
    _pc = 0;
    _sender = nullptr;
}

FrameObject::FrameObject(FunctionObject* fo) {
    _stack = new ArrayList<PObject*>();
    _loop_stack = new ArrayList<LoopBlock*>();

    _co = fo->_func_code;

    _consts = _co->_consts;
    _names = _co->_names;
    _locals = new Map<PObject*, PObject*>();
    _globals = fo->globals();

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
