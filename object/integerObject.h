#ifndef _INTEGER_OBJECT_H
#define _INTEGER_OBJECT_H

#include "pObject.h"

class IntegerKlass: public Klass {
private:
    /* private constructor */
    IntegerKlass();
    static IntegerKlass* instance;
public:
    static IntegerKlass* get_instance();

    virtual void print(PObject* x);

    virtual PObject* greater(PObject* x, PObject* y);
    virtual PObject* less   (PObject* x, PObject* y);
    virtual PObject* eq     (PObject* x, PObject* y);
    virtual PObject* ne     (PObject* x, PObject* y);
    virtual PObject* ge     (PObject* x, PObject* y);
    virtual PObject* le     (PObject* x, PObject* y);

    virtual PObject* add(PObject* x, PObject* y);
    virtual PObject* sub(PObject* x, PObject* y);
    virtual PObject* mul(PObject* x, PObject* y);
    virtual PObject* div(PObject* x, PObject* y);
    virtual PObject* mod(PObject* x, PObject* y); 
};

class IntegerObject: public PObject {
private:
    int _value;
public:
    IntegerObject(int x);
    int value();
};

#endif
