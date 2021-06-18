#ifndef _LIST_OBJECT_H
#define _LIST_OBJECT_H

#include "object.h"
#include "arrayList.h"
#include "universe.h"

/* List Klass */
class ListKlass: public Klass {
private:
    ListKlass();
    static ListKlass* instance;
public:
    static ListKlass* get_instance();

    /* native function & methods*/
    virtual void print(Object* obj);
    virtual Object* subscr(Object* obj, Object* x);
    virtual Object* contains(Object* obj, Object* x);
    virtual void store_subscr(Object* obj, Object* x, Object* index);
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

/* built in methods, should add to klass_dict */
Object* list_append(ObjList*);
Object* list_insert(ObjList* args);

#endif
