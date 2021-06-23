#ifndef _OBJECT_H
#define _OBJECT_H

#include "klass.h"

class ObjectKlass: public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;
public:
    static ObjectKlass* get_instance();
    void initialize();

    virtual void print(Object*);
};

class Object {
private:
    Klass* _klass;
    DictObject* _obj_dict;
public:
    Object();
    virtual ~Object();

    void    set_klass(Klass* x);
    void    init_dict();
    DictObject* obj_dict();

    Klass*  klass();
    
    /* operators */
    Object* add(Object* x);
    Object* sub(Object* x);
    Object* mul(Object* x);
    Object* div(Object* x);
    Object* mod(Object* x);

    Object* greater(Object* x);
    Object* less(Object* x);
    Object* eq(Object* x);
    Object* ne(Object* x);
    Object* ge(Object* x);
    Object* le(Object* x);

    /* native function */
    void print();
    Object* len();
    
    /* method */
    Object* subscr(Object* x); /* subscript */
    Object* contains(Object* x);
    void store_subscr(Object* index, Object* x);
    void del_subscr(Object* index);
    Object* getattr(Object* x);
    Object* setattr(Object* name, Object* value);
    Object* iter();

    friend class Klass;
};

bool equal2obj(Object*, Object*);

#endif
