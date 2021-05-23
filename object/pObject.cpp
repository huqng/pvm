#include "pObject.h"
#include <iostream>
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

PObject* PObject::greater(PObject* x) { return NULL; }

PObject* PObject::less(PObject* x) { return NULL; }

PObject* PObject::eq(PObject* x) { return NULL; }

PObject* PObject::ne(PObject* x) { return NULL; }

PObject* PObject::ge(PObject* x) { return NULL; }

PObject* PObject::le(PObject* x) { return NULL; }