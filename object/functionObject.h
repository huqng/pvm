#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "../code/codeObject.h"

class FunctionKlass: public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;
public:
    static FunctionKlass* get_instance();

    virtual void print(PObject* x);
};


class FunctionObject: public PObject{
private:
    CodeObject*     _func_code;
    StringObject*   _func_name;
    unsigned int    _flags;
public:
    FunctionObject(PObject* co);
    //FunctionObject(Klass* klass);

    StringObject*   func_name();
    int             flags();

    friend class FrameObject;
};

#endif
