#include "frame.h"
#include "listObject.h"
#include "dictObject.h"
#include "codeObject.h"
#include "stringObject.h"
#include "functionObject.h"

#include <cassert>

Frame::Frame(CodeObject* co) {
    _stack = new ListObject();
    _loop_stack = new ArrayList<LoopBlock*>(nullptr);

    _consts = new ListObject(co->_consts);
    _names = new ListObject(co->_names);
    _locals = new DictObject();
    _globals = _locals;
    _fast_locals = nullptr;

    _co = co;
    _pc = 0;
    _sender = nullptr;

    _locals->put(new StringObject("__name__"), new StringObject("__main__"));
}

Frame::Frame(FunctionObject* fo, ObjList* args, int op_arg) {
    /* stack */
    _stack = new ListObject();
    _loop_stack = new ArrayList<LoopBlock*>(nullptr);

    /* code */
    _co = fo->_func_code;
    _pc = 0;

    _consts = new ListObject(_co->_consts);
    _names = new ListObject(_co->_names);

    /* args */
    _locals = new DictObject();
    _globals = fo->globals();
    _fast_locals = new ListObject();
    _closure = new ListObject();

    _sender = nullptr;

    /* closure */
    if(_co->_cellvars != nullptr) {
        for(int i = 0; i < _co->_cellvars->size(); i++) {
            _closure->append(nullptr);
        }
    }
    if(fo->_closure != nullptr) {
        _closure = (ListObject*)(_closure->add(fo->_closure));
    }  

    const int nadef = _co->_argcount; /* number of args defined */
    const int nagiven = op_arg & 0xFF; /* number of args given*/
    const int nkw = op_arg >> 8; /* number of kwargs given */
    int kw_pos = nadef; /* args[argcnt] is */

    /* put default args into fast_locals*/
    int ndft = fo->_defaults->size();
    if(nagiven + ndft < nadef) {
        cerr << "error: too few arguments to call a function" << endl;
        exit(-1);
    }
    int n = (int)nadef;
    for(int i = 0; i < ndft; i++) {
        /* default args are from right to left */
        _fast_locals->set(nadef - 1 - i, fo->_defaults->get(ndft - 1 - i));
    }
    /* if given more args than defined, put them into a list */
    ListObject* alist = new ListObject();
    if(nagiven > nadef) {
        for(int i = 0; i < nadef; i++) {
            _fast_locals->set(i, args->get(i));
        }
        for(int i = nadef; i < nagiven; i++) {
            alist->append(args->get(i));
        }
    }
    else {
        for(int i = 0; i < nagiven; i++)
            _fast_locals->set(i, (args->get(i)));
    }

    /* put all kwargs into adict */
    DictObject* adict = new DictObject;
    for(int i = 0; i < nkw; i++) {
        Object* key = args->get(nagiven + i * 2);
        Object* value = args->get(nagiven + i * 2 + 1);
        adict->put(key, value);
    }

    /* flags */
    if(_co->_flag & CO_VARARGS) {
        _fast_locals->set(nadef, alist);
        kw_pos += 1;
    }
    else if(alist->size() > 0) {
        cerr << "error given more args than need" << endl;
        assert(0);
    }

    /* flags */
    if(_co->_flag & CO_VARKEYWORDS) {
        _fast_locals->set(kw_pos, adict);
    }
    else if(adict->size() > 0) {
        cerr << "error given more kwargs than need" << endl;
        assert(0);
    }
}

Frame::Frame() {
    cerr << "error building frame" << endl;
    exit(-1);

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

Object* Frame::get_cell_from_parameter(int index) {
    Object* name = _co->_cellvars->get(index);
    index = _co->_varnames->index((name));
    assert(index >= 0);
    assert(_fast_locals != nullptr);
    return _fast_locals->get(index);
}
