#include "universe.h"

integerObject* Universe::PFalse = nullptr;
integerObject* Universe::PTrue = nullptr;
PObject* Universe::PNone = nullptr;

void Universe::genesis() {
    PTrue = new integerObject(1);
    PFalse = new integerObject(0);
    PNone = new PObject();

    ObjDict* klass_dict = new ObjDict(equal2obj);
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new StringObject("upper"), new FunctionObject(string_upper));
}

void Universe::destory() {
    delete PTrue;
    delete PFalse;
    delete PNone;
}
