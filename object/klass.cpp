#include "klass.h"
#include "dictObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"
#include <cassert>

TypeObject* Klass::create_klass(Object* locals_dict, Object* supers_tuple, Object* name_str) {
    assert(locals_dict->klass() == DictKlass::get_instance());
    assert(supers_tuple->klass() == TupleKlass::get_instance());
    assert(name_str->klass() == StringKlass::get_instance());
    DictObject* locals = (DictObject*)locals_dict;
    TupleObject* supers = (TupleObject*)supers_tuple;
    StringObject* name = (StringObject*)name_str;
    
    Klass* new_klass = new Klass();
    new_klass->set_klass_dict(locals);
    new_klass->set_name(name);
    if(supers->size() > 0) {
        TypeObject* super = (TypeObject*)supers->get(0);
        new_klass->set_super(super->own_klass());
    }
    TypeObject* type_obj = new TypeObject(new_klass);
    new_klass->set_type_object(type_obj);
    return type_obj;
}

Object* Klass::allocate_instance(Object* type_obj, ObjList* args) {
    Object* inst = new Object();
    inst->set_klass(((TypeObject*)type_obj)->own_klass());
    return inst;
}

Object* Klass::allocate_instance(ObjList* args) {
    return allocate_instance(_type_obj, args);
}

