#include "universe.h"

PInteger* Universe::PFalse = nullptr;
PInteger* Universe::PTrue = nullptr;
PObject* Universe::PNone = nullptr;

void Universe::genesis() {
    PTrue = new PInteger(1);
    PFalse = new PInteger(0);
    PNone = new PObject();
}

void Universe::destory() {
    delete PTrue;
    delete PFalse;
    delete PNone;
}
