#include "functionObject.h"
#include "universe.h"
#include "stringObject.h"
#include "codeObject.h"
#include "typeObject.h"
#include "listObject.h"

#include <cassert>
#include <iostream>
using namespace std;

/* init static klass instance */
NonNativeFunctionKlass* NonNativeFunctionKlass::instance = nullptr;
NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;
MethodKlass* MethodKlass::instance = nullptr;

/* native functions */
Object* len(ObjList* args) {
    assert(args != nullptr && args->size() == 1);
    return args->get(0)->len();
}

Object* isinstance(ObjList* args) {
    assert(args != nullptr && args->size() == 2);
    assert(args->get(1)->klass() == TypeKlass::get_instance());
    Klass* type_klass = ((TypeObject*)(args->get(1)))->own_klass();
    Klass* inst_klass = args->get(0)->klass();
    ListObject* super_types = inst_klass->mro();
    for(int i = 0; i < super_types->size(); i++) {
        if(((TypeObject*)super_types->get(i))->own_klass() == inst_klass)
            return Universe::True;
    }
    return Universe::False;
}

Object* type_of(ObjList* args) {
    assert(args != nullptr && args->size() == 1);
    Object* obj = args->get(0);
    obj = obj->klass()->type_object();
    return obj;
}

/* klasses */
NonNativeFunctionKlass::NonNativeFunctionKlass() {

}

NonNativeFunctionKlass* NonNativeFunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new NonNativeFunctionKlass();
    return instance;
}

void NonNativeFunctionKlass::initialize() {
    set_name(new StringObject("NoneNativeFunction"));
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);
    add_super(ObjectKlass::get_instance());
}

void NonNativeFunctionKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "function object at " << x;
}

NativeFunctionKlass::NativeFunctionKlass() {

}

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new NativeFunctionKlass();
    return instance;
}

void NativeFunctionKlass::initialize() {
    set_name(new StringObject("NativeFunction"));
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);
    add_super(ObjectKlass::get_instance());
}

void NativeFunctionKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "native function object at" << x;
}

MethodKlass::MethodKlass() {

}

MethodKlass* MethodKlass::get_instance() {
    if(instance == nullptr)
        instance = new MethodKlass();
    return instance;
}

void MethodKlass::initialize() {
    set_name(new StringObject("Method"));
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);
    add_super(ObjectKlass::get_instance());
}

void MethodKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "method object at " << x;
}

/* function opject */ 

FunctionObject::FunctionObject(Object* x) {
    assert(x->klass() == CodeKlass::get_instance());

    CodeObject* co = (CodeObject*)x;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    _native_func = nullptr;
    set_klass(NonNativeFunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NativeFunction nfp) {
    _func_code  =   nullptr;
    _func_name  =   nullptr;
    _flags      =   0;
    _globals    =   nullptr;
    _native_func    =   nfp;
    set_klass(NativeFunctionKlass::get_instance());
}

void FunctionObject::set_defaults(ObjList* x) {
    if(x == nullptr) {
        _defaults = new ObjList(equal2obj);
        return;
    }
    _defaults = new ObjList(x->length(), equal2obj);
    for(int i = 0; i < x->length(); i++) {
        _defaults->set(i, x->get(i));
    }
}

Object* FunctionObject::call(ObjList* args) {
    return (*_native_func)(args);
}