#include "functionObject.h"
#include <cassert>
#include <iostream>
using namespace std;

/* init static klass instance */
FunctionKlass* FunctionKlass::instance = nullptr;
NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;
MethodKlass* MethodKlass::instance = nullptr;

/* native functions */
Object* len(ObjList* args) {
    return args->get(0)->len();
}

Object* string_upper(ObjList* args) {
    /* a method in class <str> */
    /* built to functionObject in Universe::genesis() */
    Object* arg = args->get(0);
    assert(arg->klass() == StringKlass::get_instance());
    StringObject* str_obj = (StringObject*)arg;
    int length = str_obj->length();
    if(length < 0)
        return Universe::None;
    
    char* new_str = new char[length];
    char c;
    for(int i = 0; i < length; i++) {
        c = str_obj->value()[i];
        if(c >= 'a' && c <= 'z')
            c += ('A' - 'a');
        new_str[i] = c;
    }
    StringObject* s = new StringObject(new_str);
    delete[] new_str;
    return s;
}

/* klasses */
FunctionKlass::FunctionKlass() {
    set_name("Function");
}

FunctionKlass* FunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new FunctionKlass();
    return instance;
}

void FunctionKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "function object at " << x;
}

NativeFunctionKlass::NativeFunctionKlass() {
    set_name("NativeFunction");
}

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new NativeFunctionKlass();
    return instance;
}

void NativeFunctionKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "native function object at" << x;
}

MethodKlass::MethodKlass() {
    set_name("Method");
}

MethodKlass* MethodKlass::get_instance() {
    if(instance == nullptr)
        instance = new MethodKlass();
    return instance;
}

void MethodKlass::print(Object* x) {
    assert(x->klass() == this);
    cout << "method object at " << x;
}

/* function opject */ 

FunctionObject::FunctionObject(Object* x) {
    assert(x->klass() == CodeObjectKlass::get_instance());

    CodeObject* co = (CodeObject*)x;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    _native_func = nullptr;
    set_klass(FunctionKlass::get_instance());
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