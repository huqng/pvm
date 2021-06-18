#include "universe.h"

IntegerObject* Universe::PFalse = nullptr;
IntegerObject* Universe::PTrue = nullptr;
Object* Universe::PNone = nullptr;

void Universe::genesis() {
    PTrue = new IntegerObject(1);
    PFalse = new IntegerObject(0);
    PNone = new Object();

    ObjDict* klass_dict = new ObjDict(equal2obj);
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new StringObject("upper"), new FunctionObject(string_upper));
}

void Universe::destory() {
    delete PTrue;
    delete PFalse;
    delete PNone;
}
