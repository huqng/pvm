#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "stringObject.h"
#include "arrayList.h"

/* copy from python2.7.7/include/code.h */
/* Masks for co_flags above */
#define CO_OPTIMIZED	0x0001
#define CO_NEWLOCALS	0x0002
#define CO_VARARGS	0x0004
#define CO_VARKEYWORDS	0x0008
#define CO_NESTED       0x0010
#define CO_GENERATOR    0x0020
/* The CO_NOFREE flag is set if there are no free or cell variables.
   This information is redundant, but it allows a single flag test
   to determine whether there is any extra work to be done when the
   call frame it setup.
*/
#define CO_NOFREE       0x0040

#if 0
/* This is no longer used.  Stopped defining in 2.5, do not re-use. */
#define CO_GENERATOR_ALLOWED    0x1000
#endif
#define CO_FUTURE_DIVISION    	0x2000
#define CO_FUTURE_ABSOLUTE_IMPORT 0x4000 /* do absolute imports by default */
#define CO_FUTURE_WITH_STATEMENT  0x8000
#define CO_FUTURE_PRINT_FUNCTION  0x10000
#define CO_FUTURE_UNICODE_LITERALS 0x20000


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
