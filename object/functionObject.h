#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "codeObject.h"
#include "map.h"
#include "universe.h"

typedef PObject* (*NativeFunction)(ObjList* args);

/* native functions */
PObject* len(ObjList* args);
PObject* string_upper(ObjList* args);

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

class MethodKlass: public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;
public:
    static MethodKlass* get_instance();
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

    NativeFunction             _native_func;

public: 
    FunctionObject(PObject* co);
    FunctionObject(NativeFunction nfp);
    //FunctionObject(Klass* klass);

    StringObject*   func_name();
    int             flags();
    Map<PObject*, PObject*>* globals() { return _globals; }
    void set_globals(Map<PObject*, PObject*>* x) { _globals = x; }
    void set_defaults(ObjList* x);

    PObject* call(ObjList* args);

    friend class Frame;
};

/* method object */
class MethodObject: public PObject {
friend class MethodKlass;
private:
    PObject* _owner;
    FunctionObject* _func;
public:
    MethodObject(FunctionObject* func): _owner(nullptr), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* func, PObject* owner): _owner(owner), _func(func) {   
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(PObject* x)  { _owner = x; }
    PObject* owner()            { return _owner; }
    FunctionObject* func()      { return _func; }
};

#endif
