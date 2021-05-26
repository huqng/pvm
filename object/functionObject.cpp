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