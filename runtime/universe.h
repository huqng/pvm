#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "object/integerObject.h"
#include "object/stringObject.h"
#include "object/functionObject.h"
#include "utils/map.h"

class IntegerObject;
class StringObject;
class FunctionObject;
class StringKlass;


class Universe {
public:
    static IntegerObject* PTrue;
    static IntegerObject* PFalse;
    static PObject* PNone;

    static void genesis();
    static void destory();
};

#endif
