#include "universe.h"

integerObject* Universe::PFalse = nullptr;
integerObject* Universe::PTrue = nullptr;
PObject* Universe::PNone = nullptr;

void Universe::genesis() {
    PTrue = new integerObject(1);
    PFalse = new integerObject(0);
    PNone = new PObject();
}

void Universe::destory() {
    delete PTrue;
    delete PFalse;
    delete PNone;
}
