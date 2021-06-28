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
    str_init = new StringObject("__init__");
    str_add = new StringObject("__add__");
    str_sub = new StringObject("__sub__");
    str_mul = new StringObject("__mul__");
    str_div = new StringObject("__div__");
    str_neg = new StringObject("__neg__");
    str_lt = new StringObject("__lt__");
    str_gt = new StringObject("__gt__");
    str_le = new StringObject("__le__");
    str_ge = new StringObject("__ge__");
    str_eq = new StringObject("__eq__");
    str_ne = new StringObject("__ne__");
    str_and = new StringObject("__and__");
    str_or = new StringObject("__or__");
    str_invert = new StringObject("__invert__");
    str_xor = new StringObject("__xor__");

    str_len = new StringObject("__len__");
    str_call = new StringObject("__call__");
    str_getattr = new StringObject("__getattr__");
    str_getitem = new StringObject("__getitem__");
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
    NonNativeFunctionKlass::get_instance()->initialize();
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
