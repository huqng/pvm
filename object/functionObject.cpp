#include "functionObject.h"
#include <cassert>
#include <iostream>
using namespace std;

FunctionKlass* FunctionKlass::instance = nullptr;

FunctionKlass::FunctionKlass() {

}

FunctionKlass* FunctionKlass::get_instance() {
    if(instance == nullptr)
        instance = new FunctionKlass();
    return instance;
}

void FunctionKlass::print(PObject* x) {
    assert(x->klass() == this);
    cout << "function object on " << x;
}

FunctionObject::FunctionObject(PObject* x) {
    assert(x->klass() == CodeObjectKlass::get_instance());

    CodeObject* co = (CodeObject*)x;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    set_klass(FunctionKlass::get_instance());
}
