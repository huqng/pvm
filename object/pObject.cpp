#include "pObject.h"
#include <iostream>
#include <cassert>
#include "universe.h"
using namespace std;

/* object klass */
ObjectKlass* ObjectKlass::instance = nullptr;

ObjectKlass::ObjectKlass() {

}

ObjectKlass* ObjectKlass::get_instance() {
    if(instance == nullptr) 
        instance = new ObjectKlass();
    return instance;
}

void ObjectKlass::print(PObject* obj) {
    assert((obj->klass() == this));
    cout << "<Object>";
}

/* Object */

PObject::PObject() {
    set_klass(ObjectKlass::get_instance());
}

PObject::~PObject() {
    
}

Klass* PObject::klass() {
    if(_klass == nullptr) {
        cerr << "error get klass: null _klass" << endl;
        exit(-1);
    }
    return _klass;
}

void PObject::set_klass(Klass* x) {
    _klass = x;
    if(_klass == nullptr) {
        cerr << "error set klass: null _klass" << endl;
        exit(-1);
    }
}

void PObject::print() {
    _klass->print(this);
}

PObject* PObject::add(PObject* x) {
    return _klass->add(this, x);
}

PObject* PObject::sub(PObject* x) {
    return _klass->sub(this, x);
}

PObject* PObject::mul(PObject* x) {
    return _klass->mul(this, x);
}

PObject* PObject::div(PObject* x) {
    return _klass->div(this, x);
}

PObject* PObject::mod(PObject* x) {
    return _klass->mod(this, x);
}

PObject* PObject::greater(PObject* x) {
    return _klass->greater(this, x);
}

PObject* PObject::less(PObject* x) {
    return _klass->less(this, x);
}

PObject* PObject::eq(PObject* x) {
    return _klass->eq(this, x);
}

PObject* PObject::ne(PObject* x) {
    return _klass->ne(this, x);
}

PObject* PObject::ge(PObject* x) {
    return _klass->ge(this, x);
}

PObject* PObject::le(PObject* x) {
    return _klass->le(this, x);
}

PObject* PObject::len() {
    return _klass->len(this);
}

PObject* PObject::subscr(PObject* x) {
    return _klass->subscr(this, x);
}

PObject* PObject::contains(PObject* x) {
    return _klass->contains(this, x);
}

bool equal2obj(PObject* a, PObject* b) {
    return a->eq(b) == Universe::PTrue;
}


PObject* PObject::getattr(PObject* x) {
    PObject* result = Universe::PNone;
    result = this->_klass->klass_dict()->get(x);
    /* 
        if attr is function, make a method, 
        else return the attr itself. 
        native function attr is put into klass_dict in Universe::genesis()
    */
    if(result->klass() == NativeFunctionKlass::get_instance() || result->klass() == FunctionKlass::get_instance()) {
        /* method and its owner */
        result = new MethodObject((FunctionObject*)result, this);
    }

    return result;
}
