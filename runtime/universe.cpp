#include "universe.h"

#include "cellObject.h"
#include "codeObject.h"
#include "dictObject.h"
#include "functionObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"

/* global string Table */

StringTable* StringTable::instance = nullptr;

StringTable::StringTable() {
    str_next = new StringObject("next");
    str_mod = new StringObject("__module__");
}

StringTable* StringTable::get_instance() {
    if(instance == nullptr)
        instance = new StringTable();
    return instance;
}
/* Universe */

IntegerObject* Universe::False = nullptr;
IntegerObject* Universe::True = nullptr;
Object* Universe::None = nullptr;

void Universe::genesis() {
    True = new IntegerObject(1);
    False = new IntegerObject(0);
    None = new Object();

    CellKlass::get_instance()->initialize();
    CodeKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    DictIteratorKlass::get_instance()->initialize();
    FunctionKlass::get_instance()->initialize();
    NativeFunctionKlass::get_instance()->initialize();
    MethodKlass::get_instance()->initialize();
    IntegerKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
    ListIteratorKlass::get_instance()->initialize();
    ObjectKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    TypeKlass::get_instance()->initialize();

}

void Universe::destory() {
    delete True;
    delete False;
    delete None;
}
