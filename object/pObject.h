#ifndef _OBJECT_H
#define _OBJECT_H

class PObject {
public:
    PObject();
    virtual ~PObject();
    
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
