#ifndef _LIST_OBJECT_H
#define _LIST_OBJECT_H

#include "klass.h"
#include "object.h"
#include "arrayList.h"

class ListKlass;
class ListIteratorKlass;
class ListIteratorObject;
class ListObject;

typedef ListKlass TupleKlass;
typedef ListIteratorKlass TupleIteratorKlass;
typedef ListIteratorObject TupleIteratorObject;
typedef ListObject TupleObject;

/* List Klass */
class ListKlass: public Klass {
private:
    ListKlass();
    static ListKlass* instance;
public:
    static ListKlass* get_instance();
    void initialize();


    virtual Object* add(Object* x, Object* y);
    virtual Object* eq(Object* x, Object* y);
    virtual Object* mul(Object* x, Object* y);
    virtual Object* len(Object* obj);

    /* native function & methods*/
    virtual void print(Object* obj);
    virtual Object* subscr(Object* obj, Object* x);
    virtual Object* contains(Object* obj, Object* x);
    virtual void store_subscr(Object* obj, Object* index, Object* x);
    virtual void del_subscr(Object* obj, Object* index);
    virtual Object* iter(Object* x);
};

/* List Iterator Klass */
class ListIteratorKlass: public Klass {
private:
    static ListIteratorKlass* instance;
    ListIteratorKlass();
public:
    static ListIteratorKlass* get_instance();
    void initialize();
};

/* List Iterator Object */
class ListIteratorObject: public Object {
private:
    ListObject* _owner;
    int _iter_cnt;
public:
    ListIteratorObject(ListObject* owner);
    ListObject* owner();
    int iter_cnt();
    void inc_cnt();
};

/* List Object */
class ListObject: public Object {
private:
    ObjList* _inner_list;
public:
    ListObject();
    ListObject(ObjList* x);
    ObjList* inner_list() { return _inner_list; }

    int size() { return _inner_list->size(); }
    void append(Object* obj) { _inner_list->append(obj); }
    Object* pop() { return _inner_list->pop(); }
    Object* get(int index);
    void set(int i, Object* obj);
    Object* top();
};

/* built in methods of class list, should add to klass_dict 
   used to build functionnObject in make_function() */
Object* list_append(ObjList* args);
Object* list_insert(ObjList* args);
Object* list_index(ObjList* args);
Object* list_pop(ObjList* args);
Object* list_remove(ObjList* args);
Object* list_reverse(ObjList* args);
Object* list_sort(ObjList* args);
Object* listiterator_next(ObjList* args);

#endif
