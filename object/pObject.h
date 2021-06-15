#ifndef _POBJECT_H
#define _POBJECT_H

#include "klass.h"

class PObject {
private:
    Klass* _klass;

public:
    PObject();
    virtual ~PObject();

    Klass*  klass();
    void    set_klass(Klass* x);
    
    void print();
    PObject* add(PObject* x);
    PObject* sub(PObject* x);
    PObject* mul(PObject* x);
    PObject* div(PObject* x);
    PObject* mod(PObject* x);

    PObject* greater(PObject* x);
    PObject* less(PObject* x);
    PObject* eq(PObject* x);
    PObject* ne(PObject* x);
    PObject* ge(PObject* x);
    PObject* le(PObject* x);

    PObject* len();
};

#endif
