#include "listObject.h"
#include "stringObject.h"
#include "dictObject.h"
#include "universe.h"
#include "functionObject.h"
#include "typeObject.h"

#include <iostream>
#include <cassert>
using namespace std;

/* list Klass */
ListKlass* ListKlass::instance = nullptr;

ListKlass::ListKlass() {

}

ListKlass* ListKlass::get_instance() {
    if(instance == nullptr)
        instance = new ListKlass();
    return instance;
}

void ListKlass::initialize() {
    set_name(new StringObject("list"));

    /* add builtin methods to klass_dict */
    DictObject* klass_dict = new DictObject();
    klass_dict->put(new StringObject("append"), new FunctionObject(list_append));
    klass_dict->put(new StringObject("insert"), new FunctionObject(list_insert));
    klass_dict->put(new StringObject("index"), new FunctionObject(list_index));
    klass_dict->put(new StringObject("pop"), new FunctionObject(list_pop));
    klass_dict->put(new StringObject("remove"), new FunctionObject(list_remove));
    klass_dict->put(new StringObject("reverse"), new FunctionObject(list_reverse));
    klass_dict->put(new StringObject("sort"), new FunctionObject(list_sort));
    set_klass_dict(klass_dict);

    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_super(ObjectKlass::get_instance());    
}

Object* ListKlass::add(Object* x, Object* y) {
    assert(x != nullptr && x->klass() == this);
    assert(y != nullptr && y->klass() == this);
    ListObject* lx = (ListObject*)x;
    ListObject* ly = (ListObject*)y;
    ListObject* lz = new ListObject();
    for(int i = 0; i < lx->size(); i++) {
        lz->append(lx->get(i));
    }
    for(int i = 0; i < ly->size(); i++) {
        lz->append(ly->get(i));
    }
    return lz;
}

Object* ListKlass::eq(Object* x, Object* y) {
    assert(x->klass() == this);
    if(y->klass() != this)
        return Universe::False;
    if(((ListObject*)y)->size() != ((ListObject*)x)->size())
        return Universe::False;
    for(int i = 0; i < ((ListObject*)x)->size(); i++) {
        Object* xi = ((ListObject*)x)->get(i);
        Object* yi = ((ListObject*)y)->get(i);
        if(xi->eq(yi) == Universe::False) {
            return Universe::False;
        }
    }
    return Universe::True;
}

Object* ListKlass::mul(Object* x, Object* y) {
    assert(x != nullptr && x->klass() == this);
    assert(y != nullptr && y->klass() == IntegerKlass::get_instance());
    ListObject* lx = (ListObject*)x;
    int times = ((IntegerObject*)y)->value();
    assert(times >= 0);
    ListObject* ret = new ListObject();
    for(int i = 0; i < times; i++) {
        for(int j = 0; j < lx->size(); j++) {
            ret->append(lx->get(j));
        }
    }
    return ret;
}

Object* ListKlass::len(Object* x) {
    assert(x && x->klass() == this);
    return new IntegerObject(((ListObject*)x)->size());
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
            return Universe::True;
        }
    }
    return Universe::False;
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

Object* ListKlass::iter(Object* obj) {
    assert(obj != nullptr && obj->klass() == this);
    ListObject* lst = (ListObject*)obj;
    return new ListIteratorObject(lst);
}

/* list iterator klass */

ListIteratorKlass* ListIteratorKlass::instance = nullptr;

ListIteratorKlass::ListIteratorKlass() {

}

ListIteratorKlass* ListIteratorKlass::get_instance() {
    if(instance == nullptr) 
        instance = new ListIteratorKlass();
    return instance;
}

void ListIteratorKlass::initialize() {
    DictObject* klass_dict = new DictObject();
    klass_dict->put(new StringObject("next"), new FunctionObject(listiterator_next));
    set_klass_dict(klass_dict);
    set_name(new StringObject("ListIterator"));
}

/* list iterator object */

ListIteratorObject::ListIteratorObject(ListObject* owner) {
    _owner = owner;
    _iter_cnt = 0;
    set_klass(ListIteratorKlass::get_instance());
}

ListObject* ListIteratorObject::owner() {
    return _owner;
}

int ListIteratorObject::iter_cnt() {
    return _iter_cnt;
}

void ListIteratorObject::inc_cnt() {
    _iter_cnt++;
}

/* list object */

ListObject::ListObject() {
    _inner_list = new ObjList(equal2obj);
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
    return Universe::None;
}

Object* list_insert(ObjList* args) {
    assert(args->size() == 3);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    assert(args->get(1)->klass() == IntegerKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0));
    IntegerObject* index = (IntegerObject*)(args->get(1));
    Object* obj = args->get(2);

    lst->inner_list()->insert(index->value(), obj);
    return Universe::None;
}

Object* list_index(ObjList* args) {
    assert(args->size() == 2);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0));

    for(int i = 0; i < lst->size(); i++) {
        if(lst->get(i)->eq(args->get(1)) == Universe::True) {
            return new IntegerObject(i);
        }
    }
    // TODO - exception
    return Universe::None;
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
        if(lst->get(i)->eq(args->get(1)) == Universe::True) {
            lst->inner_list()->delete_index(i);
            break;
        }
    }
    return Universe::None;
}

Object* list_reverse(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0));
    int lhs = 0;
    int rhs = lst->size() - 1;
    while(lhs < rhs) {
        Object* tmp = lst->get(lhs);
        lst->set(lhs, lst->get(rhs));
        lst->set(rhs, tmp);
        lhs++;
        rhs--;
    }
    return Universe::None;
}

Object* list_sort(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == ListKlass::get_instance());
    ListObject* lst = (ListObject*)(args->get(0)); 

    // TODO - sort

    return Universe::None;
}

Object* listiterator_next(ObjList* args) {
    assert(args->size() == 1);
    assert(args->get(0)->klass() == ListIteratorKlass::get_instance());
    ListIteratorObject* iter = (ListIteratorObject*)(args->get(0)); 

    ListObject* alist = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if(iter_cnt < alist->inner_list()->size()) {
        Object* obj = alist->get(iter_cnt);
        iter->inc_cnt();
        return obj;
    }
    else {
        // TODO - need StopIteration Exception
        return nullptr;
    }
}
