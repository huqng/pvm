#include "integerObject.h"
#include "universe.h"
#include "stringObject.h"
#include "typeObject.h"

#include <iostream>
#include <cassert>
using namespace std;

IntegerKlass* IntegerKlass::instance = nullptr;

IntegerKlass::IntegerKlass() {
    
}

IntegerKlass* IntegerKlass::get_instance() {
    if(instance == nullptr)
        instance = new IntegerKlass();
    return instance;
}

void IntegerKlass::initialize() {
    set_name(new StringObject("int"));
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_super(ObjectKlass::get_instance());
}

void IntegerKlass::print(Object* x) {
    assert(x->klass() == (Klass*)this);
    cout << ((IntegerObject*)x)->value();    
}

Object* IntegerKlass::add(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::None;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() + iy->value());
}

Object* IntegerKlass::sub(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::None;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() - iy->value());
}

Object* IntegerKlass::mul(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::None;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() * iy->value());
}

Object* IntegerKlass::div(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::None;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() / iy->value());
}

Object* IntegerKlass::mod(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::None;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return new IntegerObject(ix->value() % iy->value());
}

Object* IntegerKlass::greater(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() > iy->value() ? Universe::True : Universe::False;
}

Object* IntegerKlass::less(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() < iy->value() ? Universe::True : Universe::False;
}

Object* IntegerKlass::eq(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() == iy->value() ? Universe::True : Universe::False;
}

Object* IntegerKlass::ne(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() != iy->value() ? Universe::True : Universe::False;
}

Object* IntegerKlass::ge(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() >= iy->value() ? Universe::True : Universe::False;
}

Object* IntegerKlass::le(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    IntegerObject* ix = (IntegerObject*)x;
    IntegerObject* iy = (IntegerObject*)y;
    return ix->value() <= iy->value() ? Universe::True : Universe::False;
}

IntegerObject::IntegerObject(int x) {
    _value = x;
    this->set_klass(IntegerKlass::get_instance());
}

int IntegerObject::value() {
    return _value;
}
