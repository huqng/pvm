#include "dictObject.h"

/* dict klass */

DictKlass* DictKlass::instance = nullptr;

DictKlass::DictKlass() {
    set_name("Dict");
}

DictKlass* DictKlass::get_instance() {
    if(instance == nullptr);
        instance = new DictKlass();
    return instance;
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
        return Universe::PNone;
}

Object* DictKlass::contains(Object* obj, Object* key) {
    assert(obj->klass() == (Klass*)this);
    DictObject* d = (DictObject*)obj;
    if(d->map()->has_key(key))
        return Universe::PTrue;
    else
        return Universe::PFalse;
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
    ObjMap* klass_dict = new ObjMap(equal2obj);
    klass_dict->put(new StringObject("next"), new FunctionObject(dictiterator_next));
    set_klass_dict(klass_dict);
    set_name("DictIterator");
}

DictIteratorKlass* DictIteratorKlass::get_instance() {
    if(instance == nullptr)
        instance = new DictIteratorKlass();
    return instance;
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
    if(_map->has_key(k))
        return _map->get(k);
    else
        return Universe::PNone;
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
