#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "../object/pInteger.h"

class Universe {
public:
    static PInteger* PTrue;
    static PInteger* PFalse;
    static PObject* PNone;

    static void genesis();
    static void destory();
};

#endif
