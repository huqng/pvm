#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "../object/integerObject.h"

class Universe {
public:
    static integerObject* PTrue;
    static integerObject* PFalse;
    static PObject* PNone;

    static void genesis();
    static void destory();
};

#endif
