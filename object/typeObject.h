#ifndef _TYPE_OBJECT_H
#define _TYPE_OBJECT_H

#include "object.h"
#include "klass.h"
#include "arrayList.h"

class TypeKlass;
class TypeObject;

/* klass */
class TypeKlass: public Klass {
private:
    static TypeKlass* instance;
    TypeKlass();
public:
    static TypeKlass* get_instance();
    void initialize();

    virtual void print(Object* obj);    
    virtual Object* eq(Object* x, Object* y);
    virtual Object* setattr(Object* obj, Object* name, Object* value);

};

/* object */
class TypeObject: public Object {
private:
    Klass* _own_klass;  /* this is the 'Type' itself, e.g. <type: 'str'> */
                        /* and _klass is type of this typeobject, i.e. <type: 'type'> */
public:
    TypeObject(Klass* own_klass);
    //void set_own_klass(Klass* ok);
    Klass* own_klass();
};

#endif
