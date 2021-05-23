#include "pInteger.h"
#include <iostream>
#include "../runtime/universe.h"
using namespace std;

PInteger::PInteger(int x) {
    _value = x;
}

int PInteger::value() {
    return _value;
}

void PInteger::print() {
    cout << this->value();
}

PObject* PInteger::add(PObject* x) {
    return new PInteger(this->value() + ((PInteger*)x)->value());
}

PObject* PInteger::greater(PObject* x) {
    if(_value > ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}

PObject* PInteger::less(PObject* x) {
    if(_value < ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}

PObject* PInteger::eq(PObject* x) {
    if(_value == ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}

PObject* PInteger::ne(PObject* x) {
    if(_value != ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}

PObject* PInteger::ge(PObject* x) {
    if(_value >= ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}

PObject* PInteger::le(PObject* x) {
    if(_value <= ((PInteger*)x)->value())
        return Universe::PTrue;
    else
        return Universe::PFalse;
}
