#ifndef _FUNCTION_OBJECT_H
#define _FUNCTION_OBJECT_H

#include "codeObject.h"
#include "universe.h"

class ListObject;
class DictObject;

typedef Object* (*NativeFunction)(ObjList* args);

/* native functions */
Object* len(ObjList* args);
Object* string_upper(ObjList* args);

/* klasses */
class FunctionKlass: public Klass {
private:
    FunctionKlass();
    static FunctionKlass* instance;
public:
    static FunctionKlass* get_instance();
    virtual void print(Object* x);
};

class NativeFunctionKlass: public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;
public:
    static NativeFunctionKlass* get_instance();
    virtual void print(Object* x);
};

class MethodKlass: public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;
public:
    static MethodKlass* get_instance();
    virtual void print(Object* x);
};

/* Function object */
class FunctionObject: public Object{
private:
    CodeObject*     _func_code;
    StringObject*   _func_name;
    unsigned int    _flags;

    DictObject*     _globals; 
    ObjList*        _defaults;
    ListObject*     _closure;       /* closure variables */

    NativeFunction  _native_func;

public: 
    FunctionObject(Object* co);
    FunctionObject(NativeFunction nfp);
    //FunctionObject(Klass* klass);

    StringObject*   func_name();
    int             flags();
    DictObject* globals() { return _globals; }
    void set_globals(DictObject* x) { _globals = x; }
    void set_defaults(ObjList* x);
    void set_closure(ListObject* lst) { _closure = lst; }

    Object* call(ObjList* args);

    friend class Frame;
};

/* method object */
class MethodObject: public Object {
friend class MethodKlass;
private:
    Object* _owner;
    FunctionObject* _func;
public:
    MethodObject(FunctionObject* func): _owner(nullptr), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject* func, Object* owner): _owner(owner), _func(func) {   
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(Object* x)  { _owner = x; }
    Object* owner()            { return _owner; }
    FunctionObject* func()      { return _func; }
};

#endif
