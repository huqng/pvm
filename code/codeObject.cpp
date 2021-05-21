#include "codeObjet.h"

CodeObject::CodeObject(
        int argcount,
        int nlocals,
        int stacksize,
        int flag,
        PString* bytecodes,
        ArrayList<PObject*>* consts,
        ArrayList<PObject*>* names,
        ArrayList<PObject*>* varnames,
        ArrayList<PObject*>* freevars,
        ArrayList<PObject*>* cellvars,
        PString* file_name,
        PString* co_name,
        int lineno,
        PString* notable
    ):
        _argcount(argcount),
        _nlocals(nlocals),
        _stacksize(stacksize),
        _flag(flag),
        _bytecodes(bytecodes),
        _consts(consts),
        _names(names),
        _varnames(varnames),
        _freevars(freevars),
        _cellvars(cellvars),
        _file_name(file_name),
        _co_name(co_name),
        _lineno(lineno),
        _notable(notable) 
{}
