#include "universe.h"
#include "integerObject.h"

#include <iostream>
#include <cassert>
using namespace std;

IntegerKlass* IntegerKlass::instance = nullptr;

IntegerKlass::IntegerKlass() {
    set_name("Integer");
}

IntegerKlass* IntegerKlass::get_instance() {
    if(instance == nullptr)
        instance = new IntegerKlass();
    return instance;
}

void IntegerKlass::print(Object* x) {
    assert(x->klass() == (Klass*)this);
    cout << ((IntegerObject*)x)->value();    
}

Object* IntegerKlass::add(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() + iy->value());
}

Object* IntegerKlass::sub(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() - iy->value());
}

Object* IntegerKlass::mul(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() * iy->value());
}

Object* IntegerKlass::div(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() / iy->value());
}

Object* IntegerKlass::mod(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() % iy->value());
}

Object* IntegerKlass::greater(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() > iy->value() ? Universe::PTrue : Universe::PFalse;
}

Object* IntegerKlass::less(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() < iy->value() ? Universe::PTrue : Universe::PFalse;
}

Object* IntegerKlass::eq(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() == iy->value() ? Universe::PTrue : Universe::PFalse;
}

Object* IntegerKlass::ne(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() != iy->value() ? Universe::PTrue : Universe::PFalse;
}

Object* IntegerKlass::ge(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() >= iy->value() ? Universe::PTrue : Universe::PFalse;
}

Object* IntegerKlass::le(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() <= iy->value() ? Universe::PTrue : Universe::PFalse;
}

IntegerObject::IntegerObject(int x) {
    _value = x;
    this->set_klass(IntegerKlass::get_instance());
}

int IntegerObject::value() {
    return _value;
}
