#include "integerObject.h"
#include "typeObject.h"
#include "stringObject.h"
#include "dictObject.h"
#include "universe.h"
#include "oopClosure.h"

#include <cassert>
#include <iostream>
using namespace std;

/* klass */
TypeKlass* TypeKlass::instance = nullptr;

TypeKlass::TypeKlass() {
    
}

TypeKlass* TypeKlass::get_instance() {
    if(instance == nullptr)
        instance = new TypeKlass();
    return instance;
}

void TypeKlass::initialize() {
    set_name(new StringObject("type"));

    DictObject* klass_dict = new DictObject();
    set_klass_dict(klass_dict);

    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);
    add_super(ObjectKlass::get_instance());
}

Object* TypeKlass::eq(Object* x, Object* y) {
    if(x->klass() != y->klass())
        return Universe::False;
    TypeKlass* tx = (TypeKlass*)x;
    TypeKlass* ty = (TypeKlass*)y;
    if(tx == ty)    
        return Universe::True;
    else
        return Universe::False;
}

void TypeKlass::print(Object* obj) {
    assert(obj->klass() == this);
    TypeObject* tp = (TypeObject*)obj;
    cout << "<Type: ";

    /* try to print module name */
    DictObject* attr_dict = tp->own_klass()->klass_dict();
    if(attr_dict != nullptr) {
        Object* mod = StringTable::get_instance()->str_mod;
        mod = attr_dict->get(mod);
        if(mod != Universe::None) {
            mod->print();
            cout << ".";
        }
    }
    tp->own_klass()->name()->print();
    cout << ">";
}

void TypeKlass::oops_do(OopClosure* closure, Object* obj) {
    assert(obj->klass() == this);
    TypeObject* t = (TypeObject*)obj;
    closure->do_klass(t->own_klass_address());
}

Object* TypeKlass::setattr(Object* obj, Object* name, Object* value) {
    assert(obj != nullptr && obj->klass() == this);
    TypeObject* t = (TypeObject*)obj;
    t->own_klass()->klass_dict()->put(name, value);
    return Universe::None;
}

size_t TypeKlass::size() {
    return sizeof(TypeObject);
}

/* object */
TypeObject::TypeObject(Klass* own_klass) {
    /* own_klass: what this object(type object) stand for, e.g. StringKlass */
    /* klass: type of this object(type object), i.e. typeKlass */
    set_klass(TypeKlass::get_instance());
    _own_klass = own_klass;
}

//void TypeObject::set_own_klass(Klass* ok) {
//    _own_klass = ok;
//    _own_klass->set_type_object(this);
//}

Klass* TypeObject::own_klass() {
    return _own_klass;
}

Klass** TypeObject::own_klass_address() {
    return &_own_klass;
}
