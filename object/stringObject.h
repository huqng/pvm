#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "object.h"
#include "klass.h"
#include "arrayList.h"

/* native methods*/
Object* string_upper(ObjList* args);

/* klass */
class StringKlass: public Klass {
private:
    StringKlass();
    static StringKlass* instance;
public:
    static StringKlass* get_instance();
    void initialize(); /* called manually to avoid use objects in constructor of Klass */

    virtual Object* allocate_instance(ObjList* args);
    virtual void print(Object* x);

    virtual Object* greater(Object* x, Object* y);
    virtual Object* less   (Object* x, Object* y);
    virtual Object* eq     (Object* x, Object* y);
    virtual Object* ne     (Object* x, Object* y);
    virtual Object* ge     (Object* x, Object* y);
    virtual Object* le     (Object* x, Object* y);

    virtual Object* add(Object* x, Object* y);
    virtual Object* mul(Object* x, Object* y);
    virtual Object* len(Object* x);
    virtual Object* subscr(Object* obj, Object* x);
    virtual Object* contains(Object* obj, Object* x);
};

/* string object */
class StringObject: public Object {
private:
    char* _value;
    int _max_size;
public:
    StringObject(const char* x);
    StringObject(const char* x, const int length);

    const char* value();
    const int length();
};

#endif
