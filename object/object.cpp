#include "object.h"
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

void ObjectKlass::print(Object* obj) {
    assert((obj->klass() == this));
    cout << "<Object>";
}

/* Object */

Object::Object() {
    set_klass(ObjectKlass::get_instance());
}

Object::~Object() {
    
}

Klass* Object::klass() {
    if(_klass == nullptr) {
        cerr << "error get klass: null _klass" << endl;
        exit(-1);
    }
    return _klass;
}

void Object::set_klass(Klass* x) {
    _klass = x;
    if(_klass == nullptr) {
        cerr << "error set klass: null _klass" << endl;
        exit(-1);
    }
}

void Object::print() {
    _klass->print(this);
}

Object* Object::add(Object* x) {
    return _klass->add(this, x);
}

Object* Object::sub(Object* x) {
    return _klass->sub(this, x);
}

Object* Object::mul(Object* x) {
    return _klass->mul(this, x);
}

Object* Object::div(Object* x) {
    return _klass->div(this, x);
}

Object* Object::mod(Object* x) {
    return _klass->mod(this, x);
}

Object* Object::greater(Object* x) {
    return _klass->greater(this, x);
}

Object* Object::less(Object* x) {
    return _klass->less(this, x);
}

Object* Object::eq(Object* x) {
    return _klass->eq(this, x);
}

Object* Object::ne(Object* x) {
    return _klass->ne(this, x);
}

Object* Object::ge(Object* x) {
    return _klass->ge(this, x);
}

Object* Object::le(Object* x) {
    return _klass->le(this, x);
}

Object* Object::len() {
    return _klass->len(this);
}

Object* Object::subscr(Object* x) {
    return _klass->subscr(this, x);
}

Object* Object::contains(Object* x) {
    return _klass->contains(this, x);
}

bool equal2obj(Object* a, Object* b) {
    return a->eq(b) == Universe::PTrue;
}


Object* Object::getattr(Object* x) {
    Object* result = Universe::PNone;
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
