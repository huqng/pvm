#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "integerObject.h"

class Universe {
public:
    static IntegerObject* True;
    static IntegerObject* False;
    static Object* None;

    static void genesis();
    static void destory();
};

#endif
