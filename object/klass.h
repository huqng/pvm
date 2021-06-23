#ifndef _CLASS_H
#define _CLASS_H

#include "arrayList.h"

class StringObject;
class DictObject;
class Object;
class TypeObject;

class Klass {
private:
    StringObject*   _name;
    DictObject*     _klass_dict;
    TypeObject*     _type_obj;
    Klass*          _super;
public:
    Klass() {}
    ~Klass() {
        //delete _name;
        // TODO - delete obj_dict
    }

    void            set_name(StringObject* x) { _name = x; }
    StringObject*   name() { return _name; }

    void            set_klass_dict(DictObject* dict) { _klass_dict = dict; }
    DictObject*     klass_dict() { return _klass_dict; }

    void            set_type_object(TypeObject* type_obj) { _type_obj = type_obj; }
    TypeObject*     type_object() { return _type_obj; }

    void            set_super(Klass* super_klass) { _super = super_klass; }
    Klass*          super() { return _super; }

    static TypeObject* create_klass(Object* locals_dict, Object* supers_tuple, Object* name_str);
    static Object* allocate_instance(Object* type_obj, ObjList* args);

    virtual Object* allocate_instance(ObjList* args);

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
