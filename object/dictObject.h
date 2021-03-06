#ifndef _DICT_OBJECT_H
#define _DICT_OBJECT_H

#include "object.h"
#include "klass.h"
#include "arrayList.h"
#include "map.h"

class Object;
class Klass;
class OopClosure;

class DictKlass;
class DictObject;
class DictIteratorKlass;
class DictIteratorObject;

/* native methods */
/* to add to klass_dict in constructor of Klass */
Object* dict_set_default(ObjList* args);
Object* dict_remove(ObjList* args);
Object* dict_keys(ObjList* args);
Object* dict_values(ObjList* args);
Object* dict_items(ObjList* args);
Object* dictiterator_next(ObjList* args);

/* dict klass */

class DictKlass: public Klass {
private:
    DictKlass();
    static DictKlass* instance;
public:
    static DictKlass* get_instance();
    void initialize();
    
    virtual Object* allocate_instance(ObjList* args);
    virtual void print(Object* obj);
    virtual size_t size();
    virtual void oops_do(OopClosure* closure, Object* obj);
    virtual Object* subscr(Object* obj, Object* key);
    virtual Object* contains(Object* obj, Object* key);
    virtual void store_subscr(Object* obj, Object* key, Object* value);
    virtual void del_subscr(Object* obj, Object* key);
    virtual Object* iter(Object* obj);
};

/* dict iterator klass */

class DictIteratorKlass: public Klass {
private:
    DictIteratorKlass();
    static DictIteratorKlass* instance;
public:
    static DictIteratorKlass* get_instance();
    void initialize();
    virtual void oops_do(OopClosure* closure, Object* obj);
    virtual size_t size();
};

/* dict iterator */

class DictIteratorObject: public Object {
private:
    DictObject* _owner;
    int _iter_cnt;
public:
    DictIteratorObject(DictObject* owner);
    DictObject* owner();
    Object** owner_address();
    int iter_cnt();
    void inc_cnt();
};

/* dict object */

class DictObject: public Object {
private:
    ObjMap* _map;
public:
    DictObject();
    DictObject(ObjMap* map);
    ObjMap* map();
    ObjMap** map_address();

    void    put(Object* k, Object* v);
    Object* get(Object* k);
    bool    has_key(Object* k);
    int     size();
    Object* remove(Object* k);
};

#endif
