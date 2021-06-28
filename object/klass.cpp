#include "klass.h"
#include "dictObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"
#include "functionObject.h"
#include "universe.h"
#include "interpreter.h"

#include <cassert>
#include <iostream>
using namespace std;

Klass::Klass() {
    _super = nullptr;
    _mro = nullptr;
}

void Klass::add_super(Klass* klass)  {
    if(_super == nullptr)
        _super = new ListObject();
    _super->append(klass->type_object()); 
}

void Klass::order_supers() {
    if(_super == nullptr)
        return;
    if(_mro == nullptr) 
        _mro = new ListObject();
    
    int cur = -1;
    for(int i = 0; i < _super->size(); i++) {
        TypeObject* s = (TypeObject*)_super->get(i);
        Klass* k = s->klass();
        _mro->append(s);
        if(k->mro() == nullptr)
            continue;
        for(int j = 0; j < k->mro()->size(); j++) {
            TypeObject* ss = (TypeObject*)(k->mro()->get(i));
            int index = _mro->index(ss);
            if(index < cur) {
                cerr << "Error method resolution" << endl;
                assert(0);
            }
            cur = index;
            if(index >= 0)
                _mro->inner_list()->delete_index(index);
            _mro->append(ss);
        }
    }
}

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
        new_klass->set_super_list(supers);
        new_klass->order_supers();
    }
    TypeObject* type_obj = new TypeObject(new_klass);
    new_klass->set_type_object(type_obj);
    return type_obj;
}

Object* Klass::allocate_instance(Object* type_obj, ObjList* args) {
    Object* inst = new Object();
    inst->set_klass(this);
    Object* init = inst->getattr(StringTable::get_instance()->str_init);
    if(init != Universe::None) {
        Interpreter::get_instance()->call_virtual(init, args);
    }
    return inst;
}

Object* Klass::allocate_instance(ObjList* args) {
    /* if this virtual function is not rewritten in XKlass */
    /* _type_obj should have been set*/
    return allocate_instance(_type_obj, args);
}

Object* Klass::setattr(Object* obj, Object* name, Object* value) {
    /* add an attr to object, not Klass */
    /* not in TypeKlass */
    if(obj->obj_dict() == nullptr)
        obj->init_dict();
    obj->obj_dict()->put(name, value);
    return Universe::None;
}

Object* Klass::getattr(Object* obj, Object* name) {
    /* if __getattr__ overload */
    Object* func = find_in_parents(obj, StringTable::get_instance()->str_getattr);
    if(func->klass() == NonNativeFunctionKlass::get_instance()) {
        func = new MethodObject((FunctionObject*)func, obj);
        ObjList* args = new ObjList(equal2obj);
        args->append(name);
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    /* try to get from attr of object */
    Object* result = Universe::None;
    if(obj->obj_dict() != nullptr) {
        result = obj->obj_dict()->get(name);
        if(result != Universe::None)
            return result;
    }

    /* not found in attr of object, try to get from attr of class */    
    result = find_in_parents(obj, name);
    if(result->klass() == NonNativeFunctionKlass::get_instance() || result->klass() == NativeFunctionKlass::get_instance()) 
        result = new MethodObject((FunctionObject*)result, obj);
    return result;
}

Object* Klass::find_in_parents(Object* obj, Object* name) {
    Object* result = Universe::None;
    result = obj->klass()->klass_dict()->get(name);
    if(result != Universe::None)
        return result;
    /* has no super classes */
    if(_mro == nullptr)
        return result;
    for(int i = 0; i < _mro->size(); i++) {
        result = ((TypeObject*)(_mro->get(i)))->own_klass()->klass_dict()->get(name);
        if(result != Universe::None)
            return result;
    }
    return result;
}

Object* Klass::find_and_call(Object* lhs, ObjList* args, Object* func_name) {
    Object* func = lhs->getattr(func_name);
    if(func != Universe::None) {
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    cout << "Error find_and_call" << endl;
    cout << "class = ";
    lhs->klass()->name()->print();
    cout << endl << "objecct = ";
    lhs->print();
    cout << endl << "func_name = ";
    func_name->print();
    cout << endl;
    assert(0);
}

/* operator overload */
Object* Klass::add(Object* lhs, Object* rhs) {
    ObjList* args = new ObjList(equal2obj);
    args->append(rhs);
    return find_and_call(lhs, args, StringTable::get_instance()->str_add);
}

Object* Klass::sub(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_sub);
}
    
Object* Klass::mul(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_mul);
}

Object* Klass::div(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_div);
}

Object* Klass::mod(Object* x, Object* y) {
    return Universe::None; 
}

Object* Klass::neg(Object* x) {
    ObjList* args = new ObjList(equal2obj);
    return find_and_call(x, args, StringTable::get_instance()->str_neg);
}

Object* Klass::lt(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_lt);
}

Object* Klass::gt(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_gt);
}

Object* Klass::le(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_le);
}

Object* Klass::ge(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_ge);
}

Object* Klass::eq(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_eq);
}

Object* Klass::ne(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_ne);
}

Object* Klass::_and(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_and);
}

Object* Klass::_or(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_or);
}

Object* Klass::_invert(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_invert);
}

Object* Klass::_xor(Object* x, Object* y) {
    ObjList* args = new ObjList(equal2obj);
    args->append(y);
    return find_and_call(x, args, StringTable::get_instance()->str_xor);
}

Object* Klass::subscr(Object* obj, Object* index) {
    ObjList* args = new ObjList(equal2obj);
    args->append(index);
    return find_and_call(obj, args, StringTable::get_instance()->str_getitem);
}

/* native functions */
void Klass::print(Object* obj) {
    cout << "<";
    _name->print();
    cout << " at " << obj << ">";
}

Object* Klass::len(Object* x) {
    return find_and_call(x, nullptr, StringTable::get_instance()->str_len);
}
