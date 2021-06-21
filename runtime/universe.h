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
    static IntegerObject* True;
    static IntegerObject* False;
    static Object* None;

    static void genesis();
    static void destory();
};

#endif
