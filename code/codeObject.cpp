#include "codeObjet.h"
#include <iostream>
#include <iomanip>
using namespace std;

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


CodeObject::~CodeObject() {}

void CodeObject::print_info() {
    cout << setw(24) << "argcount: " << dec << _argcount << endl;
    cout << setw(24) << "nlocals: " << _nlocals << endl;
    cout << setw(24) << "stacksize: " << _stacksize << endl;
    cout << setw(24) << "flag: " << "0x" << hex << _flag << endl;

    cout << setw(24) << "bytecode: " << dec << _bytecodes->length() << endl;

    cout << setw(24) << "consts: " << _consts->size() << endl;
    cout << setw(24) << "names: " << _names->size() << endl;
    cout << setw(24) << "varnames: " << _varnames->size() << endl;
    cout << setw(24) << "freevars: " << _freevars->size() << endl;
    cout << setw(24) << "cellvars: " << _cellvars->size() << endl;

    cout << setw(24) << "filename: " << _file_name->value() << endl;
    cout << setw(24) << "co_name: " << _co_name->value() << endl;

    cout << setw(24) << "lineno: " << _lineno << endl;

    cout << setw(24) << "notable: " << _notable->value() << endl;
}
