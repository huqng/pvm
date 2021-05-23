#ifndef _P_INTEGER_H
#define _P_INTEGER_H

#include "pObject.h"

class PInteger: public PObject {
private:
    int _value;
public:
    PInteger(int x);
    int value();
    
    virtual void print();
    virtual PObject* add(PObject* x);
    
    virtual PObject* greater(PObject* x);
    virtual PObject* less(PObject* x);
    virtual PObject* eq(PObject* x);
    virtual PObject* ne(PObject* x);
    virtual PObject* ge(PObject* x);
    virtual PObject* le(PObject* x);

};

#endif
