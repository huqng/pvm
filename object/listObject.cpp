#include "listObject.h"

#include <iostream>
#include <cassert>
using namespace std;

/* list Klass */
ListKlass* ListKlass::instance = nullptr;

ListKlass::ListKlass() {
    ObjDict* klass_dict = new ObjDict(equal2obj);
    /* add builtin methods to klass_dict */
    klass_dict->put(new StringObject("append"), new FunctionObject(list_append));
    klass_dict->put(new StringObject("insert"), new FunctionObject(list_insert));
    klass_dict->put(new StringObject("index"), new FunctionObject(list_index));
    klass_dict->put(new StringObject("pop"), new FunctionObject(list_pop));
    klass_dict->put(new StringObject("remove"), new FunctionObject(list_remove));
    set_klass_dict(klass_dict);
}

ListKlass* ListKlass::get_instance() {
    if(instance == nullptr)
        instance = new ListKlass();
    return instance;
}

Object* ListKlass::eq(Object* x, Object* y) {
    assert(x->klass() == this);
    if(y->klass() != this)
        return Universe::PFalse;
    if(((ListObject*)y)->size() != ((ListObject*)x)->size())
        return Universe::PFalse;
    for(int i = 0; i < ((ListObject*)x)->size(); i++) {
        Object* xi = ((ListObject*)x)->get(i);
        Object* yi = ((ListObject*)y)->get(i);
        if(xi->eq(yi) == Universe::PFalse) {
            return Universe::PFalse;
        }
    }
    return Universe::PTrue;
}


void ListKlass::print(Object* obj) {
    assert(obj->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)obj;

    cout << "["; 

    if(lst->size() > 0)
        lst->get(0)->print();
    for(int i = 1; i < lst->size(); i++) {
        cout << ", ";
        lst->get(i)->print();
    }

    cout << "]";
}

Object* ListKlass::subscr(Object* obj, Object* x) {
    assert(obj->klass() == this);
    assert(x->klass() == IntegerKlass::get_instance());
    ListObject* lst = (ListObject*)obj;
    IntegerObject* index = (IntegerObject*)x;

    int i = (index->value()) < 0 ? (index->value() + lst->size()) : index->value();
    if(i < 0 || i >= lst->size()) {
        cerr << "list index out of range" << endl;
        exit(-1);
    }
    return lst->get(index->value());
}

Object* ListKlass::contains(Object* obj, Object* x) {
    assert(obj->klass() == this);
    ListObject* lst = (ListObject*)obj;
    for(int i = 0; i < lst->inner_list()->size(); i++) {
        if(equal2obj(lst->inner_list()->get(i), x)) {
            return Universe::PTrue;
        }
    }
    return Universe::PFalse;
}

void ListKlass::store_subscr(Object* obj, Object* index, Object* x) {
    assert(obj->klass() == this);
    assert(index->klass() == IntegerKlass::get_instance());
    ListObject* lst = (ListObject*)obj;
    IntegerObject* tmp = (IntegerObject*)index;
    int i = (tmp->value()) < 0 ? (tmp->value() + lst->size()) : tmp->value();
    lst->set(i, x);
}

void ListKlass::del_subscr(Object* obj, Object* index) {
    assert(obj->klass() == this);
    assert(index->klass() == IntegerKlass::get_instance());
    ListObject* lst = (ListObject*)obj;
    IntegerObject* tmp = (IntegerObject*)index;
    cout << "size = " << lst->size() << endl;
    cout << "index = " << tmp->value() << endl;
    int i = (tmp->value()) < 0 ? (tmp->value() + lst->size()) : tmp->value();
    lst->inner_list()->delete_index(i);
}

/* list object */
ListObject::ListObject() {
    _inner_list = new ObjList();
    set_klass(ListKlass::get_instance());
}

ListObject::ListObject(ObjList* x) {
    this->_inner_list = x;
    set_klass(ListKlass::get_instance());
}

Object* ListObject::get(int index) {
    //if(index >= this->_inner_list->size() || index < -this->_inner_list->size()) {
        /* index out of range */
    //    return Universe::PNone;
    //}
    // TODO - handle index-out-of-range exception
    if(this->_inner_list->size() == 0) {
        cerr << "err get from empty list" << endl;
        exit(-1);
    }
    return this->_inner_list->get(index % _inner_list->size());
}

void ListObject::set(int i, Object* obj) {
    this->_inner_list->set(i, obj);
}

Object* ListObject::top() {
    return this->get(this->size());
}

/* built in methods */
Object* list_append(ObjList* args) {
    assert(args->size() == 2);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ((ListObject*)(args->get(0)))->inner_list()->append(args->get(1));
    return Universe::PNone;
}

Object* list_insert(ObjList* args) {
    assert(args->size() == 3);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    assert(args->get(1)->klass() == IntegerKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0));
    IntegerObject* index = (IntegerObject*)(args->get(1));
    Object* obj = args->get(2);

    lst->inner_list()->insert(index->value(), obj);
    return Universe::PNone;
}

Object* list_index(ObjList* args) {
    assert(args->size() == 2);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0));

    for(int i = 0; i < lst->size(); i++) {
        if(lst->get(i)->eq(args->get(1)) == Universe::PTrue) {
            return new IntegerObject(i);
        }
    }
    // TODO - exception
    return Universe::PNone;
}

Object* list_pop(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)args->get(0);
    return lst->pop();
}


Object* list_remove(ObjList* args) {
    assert(args->size() == 2);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)args->get(0);
    for(int i = 0; i < lst->size(); i++) {
        if(lst->get(i)->eq(args->get(1)) == Universe::PTrue) {
            lst->inner_list()->delete_index(i);
            break;
        }
    }
    return Universe::PNone;
}

