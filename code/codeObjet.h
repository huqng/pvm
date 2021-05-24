#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "../object/stringObject.h"
#include "../utils/arrayList.h"

class CodeObject: public PObject {
protected:
    int                     _argcount;
    int                     _nlocals;
    int                     _stacksize;
    int                     _flag;
    PString*                _bytecodes;
    ArrayList<PObject*>*    _consts;
    ArrayList<PObject*>*    _names;
    ArrayList<PObject*>*    _varnames;
    ArrayList<PObject*>*    _freevars;
    ArrayList<PObject*>*    _cellvars;
    PString*                _file_name;
    PString*                _co_name;
    int                     _lineno;
    PString*                _notable;
public:
    CodeObject(
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
    );
    ~CodeObject();

    void print_details();
    
    virtual PObject* add(PObject* x);

    friend class FrameObject;
};

#endif
