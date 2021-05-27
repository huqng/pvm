#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "pObject.h"

/* klass */
class StringKlass: public Klass {
private:
    StringKlass();
    static StringKlass* instance;
public:
    static StringKlass* get_instance();

    virtual void print(PObject* x);

    virtual PObject* greater(PObject* x, PObject* y);
    virtual PObject* less   (PObject* x, PObject* y);
    virtual PObject* eq     (PObject* x, PObject* y);
    virtual PObject* ne     (PObject* x, PObject* y);
    virtual PObject* ge     (PObject* x, PObject* y);
    virtual PObject* le     (PObject* x, PObject* y);

    virtual PObject* add(PObject* x, PObject* y);
    // virtual PObject* sub(PObject* x, PObject* y);
    virtual PObject* mul(PObject* x, PObject* y);
    // virtual PObject* div(PObject* x, PObject* y);
    // virtual PObject* mod(PObject* x, PObject* y); 
    virtual PObject* len(PObject* x);
};

/* string object */
class StringObject: public PObject {
private:
    char* _value;
    int _length;
public:
    StringObject(const char* x);
    StringObject(const char* x, const int length);

    const char* value();
    const int length();
};

#endif
