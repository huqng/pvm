#ifndef _OBJECT_H
#define _OBJECT_H

#include "klass.h"

class OopClosure;

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
    long int _mark_word;
    Klass* _klass;
    DictObject* _obj_dict;
public:
    Object();
    void* operator new(size_t size);

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

    Object* gt(Object* x);
    Object* lt(Object* x);
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

    /* interfaces for gc */
    void oops_do(OopClosure* closure);
    size_t size();
    char* new_address();
    void set_new_address(char* addr);

    friend class Klass;
};

bool equal2obj(Object*, Object*);

#endif
