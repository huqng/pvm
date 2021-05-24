#include "../runtime/universe.h"
#include "pInteger.h"

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

void IntegerKlass::print(PObject* x) {
    assert(x->klass() == (Klass*)this);
    cout << ((PInteger*)x)->value();
}

PObject* IntegerKlass::add(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return new PInteger(ix->value() + iy->value());
}

PObject* IntegerKlass::sub(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return new PInteger(ix->value() - iy->value());
}

PObject* IntegerKlass::mul(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return new PInteger(ix->value() * iy->value());
}

PObject* IntegerKlass::div(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return new PInteger(ix->value() / iy->value());
}

PObject* IntegerKlass::mod(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PNone;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return new PInteger(ix->value() % iy->value());
}

PObject* IntegerKlass::greater(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() > iy->value() ? Universe::PTrue : Universe::PFalse;
}

PObject* IntegerKlass::less(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() < iy->value() ? Universe::PTrue : Universe::PFalse;
}

PObject* IntegerKlass::eq(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() == iy->value() ? Universe::PTrue : Universe::PFalse;
}

PObject* IntegerKlass::ne(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() != iy->value() ? Universe::PTrue : Universe::PFalse;
}

PObject* IntegerKlass::ge(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() >= iy->value() ? Universe::PTrue : Universe::PFalse;
}

PObject* IntegerKlass::le(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    assert(x->klass() == (Klass*)this);
    assert(y->klass() == (Klass*)this);
    PInteger* ix = (PInteger*)x;
    PInteger* iy = (PInteger*)y;
    return ix->value() <= iy->value() ? Universe::PTrue : Universe::PFalse;
}

PInteger::PInteger(int x) {
    _value = x;
    this->set_klass(IntegerKlass::get_instance());
}

int PInteger::value() {
    return _value;
}
