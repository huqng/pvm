#include "listObject.h"

#include <iostream>
#include <cassert>
using namespace std;

/* list Klass */
ListKlass* ListKlass::instance = nullptr;

ListKlass::ListKlass() {
    ObjDict* klass_dict = new ObjDict(equal2obj);
    klass_dict->put(new StringObject("append"), new FunctionObject(list_append));
    set_klass_dict(klass_dict);
}

ListKlass* ListKlass::get_instance() {
    if(instance == nullptr)
        instance = new ListKlass();
    return instance;
}

void ListKlass::print(PObject* obj) {


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

PObject* ListKlass::subscr(PObject* obj, PObject* x) {
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

PObject* ListKlass::contains(PObject* obj, PObject* x) {
    assert(obj->klass() == this);
    ListObject* lst = (ListObject*)obj;
    for(int i = 0; i < lst->inner_list()->size(); i++) {
        if(equal2obj(lst->inner_list()->get(i), x)) {
            return Universe::PTrue;
        }
    }
    return Universe::PFalse;
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

PObject* ListObject::get(int index) {
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

void ListObject::set(int i, PObject* obj) {
    this->_inner_list->set(i, obj);
}

PObject* ListObject::top() {
    return this->get(this->size());
}

/* built in methods */
PObject* list_append(ObjList* args) {
    assert(args->size() == 2);
    ((ListObject*)(args->get(0)))->inner_list()->add(args->get(1));
    return Universe::PNone;
}