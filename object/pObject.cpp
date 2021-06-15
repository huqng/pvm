#include "pObject.h"
#include <iostream>
#include "../runtime/universe.h"
using namespace std;

PObject::PObject() {

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
