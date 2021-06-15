#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "../object/stringObject.h"
#include "../utils/arrayList.h"
#include "../object/pObject.h"

class CodeObjectKlass: public Klass {
private:
    CodeObjectKlass();
    static CodeObjectKlass* instance;
public:
    static CodeObjectKlass* get_instance();
};

class CodeObject: public PObject {
protected:
    int                     _argcount;
    int                     _nlocals;
    int                     _stacksize;
    int                     _flag;
    StringObject*           _bytecodes;
    ArrayList<PObject*>*    _consts;
    ArrayList<PObject*>*    _names;
    ArrayList<PObject*>*    _varnames;
    ArrayList<PObject*>*    _freevars;
    ArrayList<PObject*>*    _cellvars;
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
        ArrayList<PObject*>* consts,
        ArrayList<PObject*>* names,
        ArrayList<PObject*>* varnames,
        ArrayList<PObject*>* freevars,
        ArrayList<PObject*>* cellvars,
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
