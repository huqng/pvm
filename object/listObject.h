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

    virtual Object* eq(Object* x, Object* y);

    /* native function & methods*/
    virtual void print(Object* obj);
    virtual Object* subscr(Object* obj, Object* x);
    virtual Object* contains(Object* obj, Object* x);
    virtual void store_subscr(Object* obj, Object* index, Object* x);
    virtual void del_subscr(Object* obj, Object* index);
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

#endif
