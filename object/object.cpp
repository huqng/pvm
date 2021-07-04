#include "universe.h"
#include "object.h"
#include "integerObject.h"
#include "stringObject.h"
#include "dictObject.h"
#include "functionObject.h"
#include "typeObject.h"
#include "heap.h"
#include "oopClosure.h"

#include <iostream>
#include <cassert>
using namespace std;

/* object klass */
ObjectKlass* ObjectKlass::instance = nullptr;

ObjectKlass::ObjectKlass() {
    
}

ObjectKlass* ObjectKlass::get_instance() {
    if(instance == nullptr) 
        instance = new ObjectKlass();
    return instance;
}

void ObjectKlass::initialize() {
    set_name(new StringObject("Object"));    
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_klass_dict(new DictObject());
    //set_super(nullptr); /* object has no super class */
}

void ObjectKlass::print(Object* obj) {
    assert((obj->klass() == this));
    cout << "<Object>";
}

/* Object */

Object::Object() {
    set_klass(ObjectKlass::get_instance());
    _obj_dict = nullptr;
}

void* Object::operator new(size_t size) {
    return Universe::heap->allocate(size);
}


void Object::init_dict() {
    _obj_dict = new DictObject();
}

DictObject* Object::obj_dict() {
    return _obj_dict;
}

Klass* Object::klass() {
    if(_klass == nullptr) {
        cerr << "error get klass: null _klass" << endl;
        exit(-1);
    }
    return _klass;
}

void Object::set_klass(Klass* x) {
    _klass = x;
    if(_klass == nullptr) {
        cerr << "error set klass: null _klass" << endl;
        exit(-1);
    }
}

void Object::print() {
    _klass->print(this);
}

Object* Object::add(Object* x) {
    return _klass->add(this, x);
}

Object* Object::sub(Object* x) {
    return _klass->sub(this, x);
}

Object* Object::mul(Object* x) {
    return _klass->mul(this, x);
}

Object* Object::div(Object* x) {
    return _klass->div(this, x);
}

Object* Object::mod(Object* x) {
    return _klass->mod(this, x);
}

Object* Object::gt(Object* x) {
    return _klass->gt(this, x);
}

Object* Object::lt(Object* x) {
    return _klass->lt(this, x);
}

Object* Object::eq(Object* x) {
    return _klass->eq(this, x);
}

Object* Object::ne(Object* x) {
    return _klass->ne(this, x);
}

Object* Object::ge(Object* x) {
    return _klass->ge(this, x);
}

Object* Object::le(Object* x) {
    return _klass->le(this, x);
}

Object* Object::len() {
    return _klass->len(this);
}

Object* Object::subscr(Object* x) {
    return _klass->subscr(this, x);
}

Object* Object::contains(Object* x) {
    return _klass->contains(this, x);
}

void Object::store_subscr(Object* index, Object* x) {
    _klass->store_subscr(this, index, x);
}

void Object::del_subscr(Object* index) {
    _klass->del_subscr(this, index);
}

bool equal2obj(Object* a, Object* b) {
    return a->eq(b) == Universe::True;
}

Object* Object::getattr(Object* name) {
    Object* result = Universe::None;
    result = _klass->getattr(this, name);
    return result;
}

Object* Object::setattr(Object* name, Object* value) {
    return _klass->setattr(this, name, value);
}

Object* Object::iter() {
    return _klass->iter(this);
}

/* gc */

void Object::oops_do(OopClosure* closure) {
    closure->do_oop((Object**)&_obj_dict);
    _klass->oops_do(closure, this);
}

size_t Object::size() {
    return _klass->size();
}

char* Object::new_address() {
    if((_mark_word & 0x2) == 0x2)
        return (char*)(_mark_word & (long)-8);
    return nullptr;
}

void Object::set_new_address(char* addr) {
    if(addr == nullptr)
        return;
    _mark_word = (long)addr | 0x2;
}


