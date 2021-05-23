#include "pObject.h"
#include <iostream>
#include "../runtime/universe.h"
using namespace std;

PObject::PObject() {

}

PObject::~PObject() {
    
}

void PObject::print() {
    cout << "Object at " << this;
}

PObject* PObject::add(PObject* x) {
    cout << "error add: Object at " << this << endl;
    exit(-1);
}

PObject* PObject::greater(PObject* x) { return Universe::PNone; }

PObject* PObject::less(PObject* x) { return Universe::PNone; }

PObject* PObject::eq(PObject* x) { return Universe::PNone; }

PObject* PObject::ne(PObject* x) { return Universe::PNone; }

PObject* PObject::ge(PObject* x) { return Universe::PNone; }

PObject* PObject::le(PObject* x) { return Universe::PNone; }