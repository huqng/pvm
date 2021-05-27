#include "functionObject.h"
#include <cassert>
#include <iostream>
using namespace std;

/* init static klass instance */
FunctionKlass* FunctionKlass::instance = nullptr;
NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

/* native functions */
PObject* len(ObjList* args) {
    return args->get(0)->len();
}

/* klasses */
FunctionKlass::FunctionKlass() {

}

FunctionKlass* FunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new FunctionKlass();
    return instance;
}

void FunctionKlass::print(PObject* x) {
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

void NativeFunctionKlass::print(PObject* x) {
    assert(x->klass() == this);
    cout << "native function object at" << x;
}

/* function opject */ 

FunctionObject::FunctionObject(PObject* x) {
    assert(x->klass() == CodeObjectKlass::get_instance());

    CodeObject* co = (CodeObject*)x;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    _native_func = nullptr;
    set_klass(FunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NFP nfp) {
    _func_code  =   nullptr;
    _func_name  =   nullptr;
    _flags      =   0;
    _globals    =   nullptr;
    _native_func    =   nfp;

    set_klass(NativeFunctionKlass::get_instance());
}

void FunctionObject::set_defaults(ArrayList<PObject*>* x) {
    if(x == nullptr) {
        _defaults = nullptr;
        return;
    }
    _defaults = new ArrayList<PObject*>(x->length());
    for(int i = 0; i < x->length(); i++) {
        _defaults->set(i, x->get(i));
    }
}

PObject* FunctionObject::call(ObjList* args) {
    return (*_native_func)(args);
}