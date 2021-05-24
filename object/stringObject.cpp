#include "../runtime/universe.h"
#include "stringObject.h"

#include <cstring>
#include <iostream>
#include <cassert>
using namespace std;

StringKlass* StringKlass::instance = nullptr;

StringKlass::StringKlass() {
    
}

StringKlass* StringKlass::get_instance() {
    if(instance == nullptr)
        instance = new StringKlass();
    return instance;
}

void StringKlass::print(PObject* x) {
    assert(x->klass() == (Klass*)this);
    for(int i = 0; i < ((StringObject*)x)->length(); i++) {
        cout << ((StringObject*)x)->value()[i];
    }
}

PObject* StringKlass::greater(PObject* x, PObject* y) {
    return 0;    
}

PObject* StringKlass::less(PObject* x, PObject* y) {
    return 0;
}

PObject* StringKlass::eq(PObject* x, PObject* y) {
    if(x->klass() != y->klass())
        return Universe::PFalse;
    StringObject* sx = (StringObject*)x;
    StringObject* sy = (StringObject*)y;

    if(sx->length() != sy->length()) {
        return Universe::PFalse;
    }

    for(int i = 0; i < sx->length(); i++) {
        if(sx->value()[i] != sy->value()[i])
            return Universe::PFalse;
    }
    return Universe::PTrue;
}

PObject* StringKlass::ne(PObject* x, PObject* y) {
    if(eq(x, y) == Universe::PFalse)
        return Universe::PTrue;
    else    
        return Universe::PFalse;
}

PObject* StringKlass::ge(PObject* x, PObject* y) {
    return 0;
}

PObject* StringKlass::le(PObject* x, PObject* y) {
    return 0;
}

PObject* StringKlass::add(PObject* x, PObject* y) {
    return 0;
}

PObject* StringKlass::mul(PObject* x, PObject* y) {
    return 0;
}


StringObject::StringObject(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);
    this->set_klass(StringKlass::get_instance());
}

StringObject::StringObject(const char* x, const int length) {
    _length = length;
    _value = new char[_length];
    memcpy(_value, x, _length);
    this->set_klass(StringKlass::get_instance());
}

const char* StringObject::value() {
    return _value;
}

const int StringObject::length() {
    return _length;
}
