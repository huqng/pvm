#include "dictObject.h"
#include "universe.h"
#include "listObject.h"
#include "stringObject.h"
#include "functionObject.h"
#include "typeObject.h"

#include <cassert>
#include <iostream>
using namespace std;

/* native methods */

Object* dict_set_default(ObjList* args) {
    assert(args->size() == 3);
    assert(args->get(0)->klass() == (Klass*)(DictKlass::get_instance()));
    DictObject* dict = (DictObject*)(args->get(0));
    Object* key = args->get(1);
    Object* value = args->get(2);
    
    if(!dict->has_key(key))
        dict->put(key, value);
    return Universe::None;
}

Object* dict_remove(ObjList* args) {
    assert(args->size() == 2);
    assert(args->get(0)->klass() == (Klass*)(DictKlass::get_instance()));
    DictObject* dict = (DictObject*)(args->get(0));
    Object* key = args->get(1);
    dict->remove(key);
    return Universe::None;
}

Object* dict_keys(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == (Klass*)(DictKlass::get_instance()));
    DictObject* dict = (DictObject*)(args->get(0));
    ListObject* keys = new ListObject();
    for(int i = 0; i < dict->size(); i++)
        keys->append(dict->map()->get_key(i));
    return keys;
}

Object* dict_values(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == (Klass*)(DictKlass::get_instance()));
    DictObject* dict = (DictObject*)(args->get(0));
    ListObject* values = new ListObject();
    for(int i = 0; i < dict->size(); i++) {
        Object* value = dict->map()->get(dict->map()->get_key(i));
        values->append(value);
    }
    
    return values;
}

Object* dict_items(ObjList* args) {
    /* TODO - use tuple instead of list */
    assert(args->size() == 1);
    assert(args->get(0)->klass() == (Klass*)(DictKlass::get_instance()));
    DictObject* dict = (DictObject*)(args->get(0));
    ListObject* items = new ListObject();
    TupleObject* item = nullptr;
    for(int i = 0; i < dict->size(); i++) {
        item = new TupleObject();
        Object* key = dict->map()->get_key(i);
        Object* value = dict->map()->get(key);
        item->append(key);
        item->append(value);
        items->append(item);
    }
    return items;
}

Object* dictiterator_next(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == (Klass*)(DictIteratorKlass::get_instance()));
    DictIteratorObject* iter = (DictIteratorObject*)(args->get(0)); 

    DictObject* dict = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if(iter_cnt < dict->size()) {
        Object* key = dict->map()->get_key(iter_cnt);
        iter->inc_cnt();
        return key;
    }
    else {
        // TODO - need StopIteration Exception
        return nullptr;
    }
}

/* dict klass */

DictKlass* DictKlass::instance = nullptr;

DictKlass::DictKlass() {

}

DictKlass* DictKlass::get_instance() {
    if(instance == nullptr)
        instance = new DictKlass();
    return instance;
}

void DictKlass::initialize() {
    set_name(new StringObject("Dict"));

    DictObject* klass_dict = new DictObject();
    /* add builtin methods to klass_dict */
    klass_dict->put(new StringObject("setdefault"), new FunctionObject(dict_set_default));
    klass_dict->put(new StringObject("remove"), new FunctionObject(dict_remove));
    klass_dict->put(new StringObject("keys"), new FunctionObject(dict_keys));
    klass_dict->put(new StringObject("values"), new FunctionObject(dict_values));
    klass_dict->put(new StringObject("items"), new FunctionObject(dict_items));
    //klass_dict->put(new StringObject("METHOD_NAME"), new FunctionObject(METHOD_FUNCTION_POINTER));
    set_klass_dict(klass_dict);

    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_super(ObjectKlass::get_instance());
}

Object* DictKlass::allocate_instance(ObjList* args) {
    return new DictObject();
}

void DictKlass::print(Object* obj) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    cout << "{";
    if(d->map()->size() > 0) {
        Object* key = d->map()->get_key(0);
        key->print();
        cout << ": ";
        Object* value = d->map()->get(key);
        value->print();
    }
    for(int i = 1; i < d->map()->size(); i++) {
        cout << ", ";
        Object* key = d->map()->get_key(i);
        key->print();
        cout << ": ";
        Object* value = d->map()->get(key);
        value->print();
    }
    cout << "}";
}

Object* DictKlass::subscr(Object* obj, Object* key) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    if(d->map()->has_key(key) )
        return d->map()->get(key);
    else
        return Universe::None;
}

Object* DictKlass::contains(Object* obj, Object* key) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    if(d->map()->has_key(key))
        return Universe::True;
    else
        return Universe::False;
}

void DictKlass::store_subscr(Object* obj, Object* key, Object* value) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    d->put(key, value);
}

void DictKlass::del_subscr(Object* obj, Object* key) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    d->map()->remove(key);
}

Object* DictKlass::iter(Object* obj) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    return new DictIteratorObject(d);
}

/* Dict Iterator Klass */

DictIteratorKlass* DictIteratorKlass::instance = nullptr;

DictIteratorKlass::DictIteratorKlass() {

}

DictIteratorKlass* DictIteratorKlass::get_instance() {
    if(instance == nullptr)
        instance = new DictIteratorKlass();
    return instance;
}

void DictIteratorKlass::initialize() {
    DictObject* klass_dict = new DictObject();
    klass_dict->put(new StringObject("next"), new FunctionObject(dictiterator_next));
    set_klass_dict(klass_dict);
    set_name(new StringObject("DictIterator"));

    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);
}

/* dict iterator object */
DictIteratorObject::DictIteratorObject(DictObject* owner) {
    _owner = owner;
    _iter_cnt = 0;
    set_klass(DictIteratorKlass::get_instance());
}

DictObject* DictIteratorObject::owner() {
    return _owner;
}

int DictIteratorObject::iter_cnt() {
    return _iter_cnt;
}

void DictIteratorObject::inc_cnt() {
    _iter_cnt++;
}

/* dict object */

DictObject::DictObject() {
    _map = new ObjMap(equal2obj);
    set_klass(DictKlass::get_instance());
}

DictObject::DictObject(ObjMap* map) {
    _map = map;
    set_klass(DictKlass::get_instance());
}

ObjMap* DictObject::map() {
    return _map;
}

void DictObject::put(Object* k, Object* v) {
    _map->put(k, v);
}

Object* DictObject::get(Object* k) {
    if(_map->has_key(k)) {
        return _map->get(k);
    }
    else {
        return Universe::None;
    }
}

bool DictObject::has_key(Object* k) {
    return _map->has_key(k);
}

int DictObject::size() {
    return _map->size();
}

Object* DictObject::remove(Object* k) {
    return _map->remove(k);
}
