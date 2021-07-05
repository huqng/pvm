#ifndef _CODE_OBJECT_H
#define _CODE_OBJECT_H

#include "klass.h"
#include "object.h"

class StringObject;
class Frame;
class FunctionObject;
class ListObject;
class OopClosure;

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


class CodeKlass: public Klass {
private:
    CodeKlass();
    static CodeKlass* instance;
public:
    static CodeKlass* get_instance();
    void initialize();

    virtual void print(Object* x);
    virtual size_t size();
    virtual void oops_do(OopClosure* closure, Object* obj);
};

class CodeObject: public Object {
public:
    int _argcount;
    int _nlocals;
    int _stacksize;
    int _flag;
    StringObject*   _bytecodes;
    ListObject*        _consts;
    ListObject*        _names;
    ListObject*        _varnames;
    ListObject*        _freevars;
    ListObject*        _cellvars;
    StringObject*   _file_name;
    StringObject*   _co_name;
    int             _lineno;
    StringObject*   _notable;
public:
    CodeObject(
        int argcount,
        int nlocals,
        int stacksize,
        int flag,
        StringObject* bytecodes,
        ListObject* consts,
        ListObject* names,
        ListObject* varnames,
        ListObject* freevars,
        ListObject* cellvars,
        StringObject* file_name,
        StringObject* co_name,
        int lineno,
        StringObject* notable
    );
    ~CodeObject();

    void print_details();
    
    friend class Frame;
    friend class FunctionObject;
    friend class CodeKlass;
};

#endif
