#include "universe.h"

IntegerObject* Universe::False = nullptr;
IntegerObject* Universe::True = nullptr;
Object* Universe::None = nullptr;

void Universe::genesis() {
    True = new IntegerObject(1);
    False = new IntegerObject(0);
    None = new Object();

    ObjMap* klass_dict = new ObjMap(equal2obj);
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new StringObject("upper"), new FunctionObject(string_upper));
}

void Universe::destory() {
    delete True;
    delete False;
    delete None;
}
