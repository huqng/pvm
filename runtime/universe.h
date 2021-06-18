#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "integerObject.h"
#include "stringObject.h"
#include "functionObject.h"
#include "map.h"

class IntegerObject;
class StringObject;
class FunctionObject;
class StringKlass;

class Universe {
public:
    static IntegerObject* PTrue;
    static IntegerObject* PFalse;
    static Object* PNone;

    static void genesis();
    static void destory();
};

#endif
