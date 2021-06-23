#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "object.h"

class IntegerKlass: public Klass {
private:
    /* private constructor */
    IntegerKlass();
    static IntegerKlass* instance;
public:
    static IntegerKlass* get_instance();
    void initialize();

    virtual void print(Object* x);

    virtual Object* greater(Object* x, Object* y);
    virtual Object* less   (Object* x, Object* y);
    virtual Object* eq     (Object* x, Object* y);
    virtual Object* ne     (Object* x, Object* y);
    virtual Object* ge     (Object* x, Object* y);
    virtual Object* le     (Object* x, Object* y);

    virtual Object* add(Object* x, Object* y);
    virtual Object* sub(Object* x, Object* y);
    virtual Object* mul(Object* x, Object* y);
    virtual Object* div(Object* x, Object* y);
    virtual Object* mod(Object* x, Object* y); 
};

class IntegerObject: public Object {
private:
    int _value;
public:
    IntegerObject(int x);
    int value();
};

#endif
