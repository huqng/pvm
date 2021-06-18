#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "stringObject.h"
#include "arrayList.h"

class CodeObjectKlass: public Klass {
private:
    CodeObjectKlass();
    static CodeObjectKlass* instance;
public:
    static CodeObjectKlass* get_instance();
};

class CodeObject: public Object {
protected:
    int                     _argcount;
    int                     _nlocals;
    int                     _stacksize;
    int                     _flag;
    StringObject*           _bytecodes;
    ArrayList<Object*>*    _consts;
    ArrayList<Object*>*    _names;
    ArrayList<Object*>*    _varnames;
    ArrayList<Object*>*    _freevars;
    ArrayList<Object*>*    _cellvars;
    StringObject*           _file_name;
    StringObject*           _co_name;
    int                     _lineno;
    StringObject*           _notable;
public:
    CodeObject(
        int argcount,
        int nlocals,
        int stacksize,
        int flag,
        StringObject* bytecodes,
        ArrayList<Object*>* consts,
        ArrayList<Object*>* names,
        ArrayList<Object*>* varnames,
        ArrayList<Object*>* freevars,
        ArrayList<Object*>* cellvars,
        StringObject* file_name,
        StringObject* co_name,
        int lineno,
        StringObject* notable
    );
    ~CodeObject();

    void print_details();
    
    friend class Frame;
    friend class FunctionObject;
};

#endif
