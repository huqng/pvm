#include "klass.h"
#include "dictObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"
#include "functionObject.h"
#include "universe.h"

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
    /* if this virtual function is not rewritten in XKlass */
    return allocate_instance(_type_obj, args);
}

Object* Klass::setattr(Object* obj, Object* name, Object* value) {
    if(obj->obj_dict() == nullptr)
        obj->init_dict();
    obj->obj_dict()->put(name, value);
    return Universe::None;
}

Object* Klass::getattr(Object* obj, Object* name) {
    Object* result = Universe::None;
    if(obj->obj_dict() != nullptr) {
        result = obj->obj_dict()->get(name);
        if(result != Universe::None)
            return result;
    }
    result = this->_klass_dict->get(name);
    if(result->klass() == NonNativeFunctionKlass::get_instance() || result->klass() == NativeFunctionKlass::get_instance()) 
        result = new MethodObject((FunctionObject*)result, obj);
    return result;
}
