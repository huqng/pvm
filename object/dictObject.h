#ifndef _DICT_OBJECT_H
#define _DICT_OBJECT_H

#include "object.h"
#include "cassert"
#include "universe.h"
#include "listObject.h"
#include <iostream>
using namespace std;

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

    virtual void print(Object* obj);
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
};

/* dict iterator */

class DictIteratorObject: public Object {
private:
    DictObject* _owner;
    int _iter_cnt;
public:
    DictIteratorObject(DictObject* owner);
    DictObject* owner();
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

    void    put(Object* k, Object* v);
    Object* get(Object* k);
    bool    has_key(Object* k);
    int     size();
    Object* remove(Object* k);
};

#endif
