#include "codeObject.h"
#include <iostream>
#include <iomanip>
using namespace std;

/* code object klass */
CodeObjectKlass* CodeObjectKlass::instance = nullptr;

CodeObjectKlass::CodeObjectKlass() {

}

CodeObjectKlass* CodeObjectKlass::get_instance() {
    if(instance == nullptr)
        instance = new CodeObjectKlass();
    return instance;
}

/* code object initialize */
CodeObject::CodeObject(
        int argcount,
        int nlocals,
        int stacksize,
        int flag,
        StringObject* bytecodes,
        ArrayList<PObject*>* consts,
        ArrayList<PObject*>* names,
        ArrayList<PObject*>* varnames,
        ArrayList<PObject*>* freevars,
        ArrayList<PObject*>* cellvars,
        StringObject* file_name,
        StringObject* co_name,
        int lineno,
        StringObject* notable
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
        _notable(notable) {
    set_klass(CodeObjectKlass::get_instance());
}


CodeObject::~CodeObject() {}

void CodeObject::print_details() {
    cout << setw(20) << "Code Object" << "at " << this << ":" << endl;
    cout << "\t" << setw(12) << "argcount: " << dec << _argcount << endl;
    cout << "\t" << setw(12) << "nlocals: " << _nlocals << endl;
    cout << "\t" << setw(12) << "stacksize: " << _stacksize << endl;
    cout << "\t" << setw(12) << "flag: " << "0x" << hex << _flag << endl;

    cout << "\t" << setw(12) << "bytecode: " << dec << _bytecodes->length() << endl;

    cout << "\t" << setw(12) << "consts: " << _consts->size() << endl;
    cout << "\t" << setw(12) << "names: " << _names->size() << endl;
    cout << "\t" << setw(12) << "varnames: " << _varnames->size() << endl;
    cout << "\t" << setw(12) << "freevars: " << _freevars->size() << endl;
    cout << "\t" << setw(12) << "cellvars: " << _cellvars->size() << endl;

    cout << "\t" << setw(12) << "filename: " << _file_name->value() << endl;
    cout << "\t" << setw(12) << "co_name: " << _co_name->value() << endl;

    cout << "\t" << setw(12) << "lineno: " << _lineno << endl;

    cout << "\t" << setw(12) << "notable: " << "[length = " << _notable->length() << "]" << endl;
    cout << "================= Parse succeed ===============" << endl;
}
