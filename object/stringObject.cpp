#include "runtime/universe.h"
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

void StringKlass::print(Object* x) {
    assert(x->klass() == (Klass*)this);
    for(int i = 0; i < ((StringObject*)x)->length(); i++) {
        cout << ((StringObject*)x)->value()[i];
    }
}

Object* StringKlass::greater(Object* x, Object* y) {
    return 0;    
}

Object* StringKlass::less(Object* x, Object* y) {
    return 0;
}

Object* StringKlass::eq(Object* x, Object* y) {
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

Object* StringKlass::ne(Object* x, Object* y) {
    if(eq(x, y) == Universe::PFalse)
        return Universe::PTrue;
    else    
        return Universe::PFalse;
}

Object* StringKlass::ge(Object* x, Object* y) {
    return 0;
}

Object* StringKlass::le(Object* x, Object* y) {
    return 0;
}

Object* StringKlass::add(Object* x, Object* y) {
    return 0;
}

Object* StringKlass::mul(Object* x, Object* y) {
    return 0;
}

Object* StringKlass::len(Object* x) {
    assert(x->klass() == this);
    return new IntegerObject(((StringObject*)x)->length());
}

Object* StringKlass::subscr(Object* obj, Object* x) {
    assert(obj->klass() == this);
    assert(x->klass() == IntegerKlass::get_instance());
    StringObject* s = (StringObject*)obj;
    IntegerObject* index = (IntegerObject*)x;

    int i = (index->value()) < 0 ? (index->value() + s->length()) : index->value();
    if(i < 0 || i >= s->length()) {
        cerr << "string index out of range" << endl;
        exit(-1);
    }

    return new StringObject(s->value() + i, 1);
}

Object* StringKlass::contains(Object* obj, Object* x) {
    assert(obj->klass() == this);
    if(x->klass() != this)
        return Universe::PFalse;
    else {
        StringObject* s = (StringObject*)obj;
        StringObject* subs = (StringObject*)x;
        if(strstr(s->value(), subs->value()) != nullptr)
            return Universe::PTrue;
        else
            return Universe::PFalse;
    }
    
}


/* string object */
StringObject::StringObject(const char* x) {
    _max_size = strlen(x);
    _value = new char[_max_size];
    strcpy(_value, x);
    this->set_klass(StringKlass::get_instance());
}

StringObject::StringObject(const char* x, const int length) {
    _max_size = length;
    _value = new char[_max_size];
    memcpy(_value, x, _max_size);
    this->set_klass(StringKlass::get_instance());
}

const char* StringObject::value() {
    return _value;
}

const int StringObject::length() {
    return _max_size;
}
