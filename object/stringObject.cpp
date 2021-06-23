#include "universe.h"
#include "stringObject.h"
#include "dictObject.h"
#include "functionObject.h"
#include "typeObject.h"

#include <cstring>
#include <iostream>
#include <cassert>
using namespace std;

/* native methods of str */
Object* string_upper(ObjList* args) {
    /* a method in class <str> */
    /* built to functionObject in Universe::genesis() */
    Object* arg = args->get(0);
    assert(arg->klass() == StringKlass::get_instance());
    StringObject* str_obj = (StringObject*)arg;
    int length = str_obj->length();
    if(length < 0)
        return Universe::None;
    
    char* new_str = new char[length];
    char c;
    for(int i = 0; i < length; i++) {
        c = str_obj->value()[i];
        if(c >= 'a' && c <= 'z')
            c += ('A' - 'a');
        new_str[i] = c;
    }
    StringObject* s = new StringObject(new_str);
    delete[] new_str;
    return s;
}

/* klass */

StringKlass* StringKlass::instance = nullptr;

StringKlass::StringKlass() { 
    
}

StringKlass* StringKlass::get_instance() {
    if(instance == nullptr)
        instance = new StringKlass();
    return instance;
}

void StringKlass::initialize() {
    set_name(new StringObject("str"));
    
    DictObject* klass_dict = new DictObject();
    klass_dict->put(new StringObject("upper"), new FunctionObject(string_upper));
    set_klass_dict(klass_dict);
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_super(ObjectKlass::get_instance());
}

Object* StringKlass::allocate_instance(ObjList* args) {
    return new StringObject("");
}

void StringKlass::print(Object* x) {
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
        return Universe::False;
    StringObject* sx = (StringObject*)x;
    StringObject* sy = (StringObject*)y;

    if(sx->length() != sy->length()) {
        return Universe::False;
    }

    for(int i = 0; i < sx->length(); i++) {
        if(sx->value()[i] != sy->value()[i])
            return Universe::False;
    }
    return Universe::True;
}

Object* StringKlass::ne(Object* x, Object* y) {
    if(eq(x, y) == Universe::False)
        return Universe::True;
    else    
        return Universe::False;
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
        return Universe::False;
    else {
        StringObject* s = (StringObject*)obj;
        StringObject* subs = (StringObject*)x;
        if(strstr(s->value(), subs->value()) != nullptr)
            return Universe::True;
        else
            return Universe::False;
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
