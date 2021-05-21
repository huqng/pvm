#ifndef _P_INTEGER_H
#define _P_INTEGER_H

#include "pObject.h"

class PInteger: public PObject {
private:
    int _value;
public:
    PInteger(int x);
    int value();

};

#endif
