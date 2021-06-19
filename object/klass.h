#ifndef _CLASS_H
#define _CLASS_H

#include "map.h"
#include "arrayList.h"

class Object;

class Klass {
private:
    const char* _name;
    ObjDict* _klass_dict;
public:
    Klass() {}
    ~Klass() {
        delete[] _name;
        // TODO - delete obj_dict
    }

    void        set_name(const char* x) { _name = x; }
    const char* name() { return _name; }

    void        set_klass_dict(ObjDict* dict) { _klass_dict = dict; }
    ObjDict*    klass_dict() { return _klass_dict; }



    virtual Object* greater(Object* x, Object* y) { return nullptr; }
    virtual Object* less   (Object* x, Object* y) { return nullptr; }
    virtual Object* eq     (Object* x, Object* y) { return nullptr; }
    virtual Object* ne     (Object* x, Object* y) { return nullptr; }
    virtual Object* ge     (Object* x, Object* y) { return nullptr; }
    virtual Object* le     (Object* x, Object* y) { return nullptr; }

    virtual Object* add(Object* x, Object* y) { return nullptr; }
    virtual Object* sub(Object* x, Object* y) { return nullptr; }
    virtual Object* mul(Object* x, Object* y) { return nullptr; }
    virtual Object* div(Object* x, Object* y) { return nullptr; }
    virtual Object* mod(Object* x, Object* y) { return nullptr; }

    virtual void print(Object* obj) {}
    virtual Object* len(Object* x) { return nullptr; }
    
    virtual Object* subscr(Object* obj, Object* index) { return nullptr; }
    virtual Object* contains(Object* obj, Object* x) { return nullptr; }
    virtual void store_subscr(Object* obj, Object* index, Object* x) { }
    virtual void del_subscr(Object* obj, Object* index) { }
    virtual Object* iter(Object* obj) { return nullptr; }
};

#endif
