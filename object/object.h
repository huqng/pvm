#ifndef _OBJECT_H
#define _OBJECT_H

#include "klass.h"

class ObjectKlass: public Klass {
private:
    ObjectKlass();
    static ObjectKlass* instance;
public:
    static ObjectKlass* get_instance();
    virtual void print(Object*);
};


class Object {
private:
    Klass* _klass;

public:
    Object();
    virtual ~Object();

    Klass*  klass();
    void    set_klass(Klass* x);
    
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
    Object* iter();

};

bool equal2obj(Object*, Object*);

#endif
