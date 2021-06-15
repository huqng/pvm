#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "../object/codeObject.h"
#include "../utils/map.h"

typedef PObject* (*NFP)(ObjList* args);

/* native functions */
PObject* len(ObjList* args);

/* klasses */
class FunctionKlass: public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;
public:
    static FunctionKlass* get_instance();
    virtual void print(PObject* x);
};

class NativeFunctionKlass: public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;
public:
    static NativeFunctionKlass* get_instance();
    virtual void print(PObject* x);
};

/* Function object */
class FunctionObject: public PObject{
private:
    CodeObject*     _func_code;
    StringObject*   _func_name;
    unsigned int    _flags;

    Map<PObject*, PObject*>* _globals; 
    ObjList*        _defaults;

    NFP             _native_func;

public: 
    FunctionObject(PObject* co);
    FunctionObject(NFP nfp);
    //FunctionObject(Klass* klass);

    StringObject*   func_name();
    int             flags();
    Map<PObject*, PObject*>* globals() { return _globals; }
    void set_globals(Map<PObject*, PObject*>* x) { _globals = x; }
    void set_defaults(ObjList* x);

    PObject* call(ObjList* args);

    friend class Frame;
};

#endif
