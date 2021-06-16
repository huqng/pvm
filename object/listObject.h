#ifndef _LIST_OBJECT_H
#define _LIST_OBJECT_H

#include "pObject.h"
#include "arrayList.h"
#include "universe.h"

/* List Klass */
class ListKlass: public Klass {
private:
    ListKlass();
    static ListKlass* instance;
public:
    static ListKlass* get_instance();

    virtual void print(PObject* obj);
    virtual PObject* subscr(PObject* lst, PObject* index);
};

/* List Object */
class ListObject: public PObject {
private:
    ObjList* _inner_list;
public:
    ListObject();
    ListObject(ObjList* x);
    ObjList* inner_list() { return _inner_list; }

    int size() { return _inner_list->size(); }
    void append(PObject* obj) { _inner_list->add(obj); }
    PObject* pop() { return _inner_list->pop(); }
    PObject* get(int index);
    void set(int i, PObject* obj);
    PObject* top();
};

#endif
