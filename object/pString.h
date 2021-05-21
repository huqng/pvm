#ifndef _P_STRING_H
#define _P_STRING_H

#include "pObject.h"

class PString: public PObject {
private:
    char* _value;
    int _length;
public:
    PString(const char* x);
    PString(const char* x, const int length);

    const char* value();
    const int length();
};

#endif
