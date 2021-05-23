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

void CodeObject::print() {
    cout << "Code Object at " << this << ":" << endl;
    cout << "\t" << setw(16) << "argcount: " << dec << _argcount << endl;
    cout << "\t" << setw(16) << "nlocals: " << _nlocals << endl;
    cout << "\t" << setw(16) << "stacksize: " << _stacksize << endl;
    cout << "\t" << setw(16) << "flag: " << "0x" << hex << _flag << endl;

    cout << "\t" << setw(16) << "bytecode: " << dec << _bytecodes->length() << endl;

    cout << "\t" << setw(16) << "consts: " << _consts->size() << endl;
    cout << "\t" << setw(16) << "names: " << _names->size() << endl;
    cout << "\t" << setw(16) << "varnames: " << _varnames->size() << endl;
    cout << "\t" << setw(16) << "freevars: " << _freevars->size() << endl;
    cout << "\t" << setw(16) << "cellvars: " << _cellvars->size() << endl;

    cout << "\t" << setw(16) << "filename: " << _file_name->value() << endl;
    cout << "\t" << setw(16) << "co_name: " << _co_name->value() << endl;

    cout << "\t" << setw(16) << "lineno: " << _lineno << endl;

    cout << "\t" << setw(16) << "notable: " << _notable->value() << endl;
}

PObject* CodeObject::add(PObject* x) {
    cout << "error add: Code Object at " << this << endl;
    exit(-1);
}