#include "universe.h"

#include "arrayList.h"
#include "integerObject.h"
#include "cellObject.h"
#include "codeObject.h"
#include "dictObject.h"
#include "functionObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"
#include "oopClosure.h"
#include "heap.h"

#include <iostream>
using namespace std;

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

void StringTable::oops_do(OopClosure* closure) {
    closure->do_oop((Object**)&str_next);
    closure->do_oop((Object**)&str_mod);
    closure->do_oop((Object**)&str_init);

    closure->do_oop((Object**)&str_add);
    closure->do_oop((Object**)&str_sub);
    closure->do_oop((Object**)&str_mul);
    closure->do_oop((Object**)&str_div);
    closure->do_oop((Object**)&str_neg);
    closure->do_oop((Object**)&str_lt);
    closure->do_oop((Object**)&str_gt);
    closure->do_oop((Object**)&str_le);
    closure->do_oop((Object**)&str_ge);
    closure->do_oop((Object**)&str_eq);
    closure->do_oop((Object**)&str_ne);
    closure->do_oop((Object**)&str_and);
    closure->do_oop((Object**)&str_or);
    closure->do_oop((Object**)&str_invert);
    closure->do_oop((Object**)&str_xor);
    closure->do_oop((Object**)&str_len);
    closure->do_oop((Object**)&str_call);
    closure->do_oop((Object**)&str_getattr);
    closure->do_oop((Object**)&str_getitem);
}

/* Universe */

CodeObject* Universe::main_code = nullptr;
IntegerObject* Universe::False = nullptr;
IntegerObject* Universe::True = nullptr;
Object* Universe::None = nullptr;
Heap* Universe::heap = nullptr;
ArrayList<Klass*>* Universe::klasses = nullptr;

void Universe::genesis() {
    /* initialize heap before everything */
    heap = Heap::get_instance();
    
    klasses = new ArrayList<Klass*>();
    True = new IntegerObject(1);
    False = new IntegerObject(0);
    None = new Object();

    ObjectKlass::get_instance()->initialize();
    TypeKlass::get_instance()->initialize();
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
    StringKlass::get_instance()->initialize();

    CellKlass::get_instance()->order_supers();
    CodeKlass::get_instance()->order_supers();
    DictKlass::get_instance()->order_supers();
    DictIteratorKlass::get_instance()->order_supers();
    NonNativeFunctionKlass::get_instance()->order_supers();
    NativeFunctionKlass::get_instance()->order_supers();
    MethodKlass::get_instance()->order_supers();
    IntegerKlass::get_instance()->order_supers();
    ListKlass::get_instance()->order_supers();
    ListIteratorKlass::get_instance()->order_supers();
    ObjectKlass::get_instance()->order_supers();
    StringKlass::get_instance()->order_supers();
    TypeKlass::get_instance()->order_supers();

}

void Universe::destory() {
    //delete True;
    //delete False;
    //delete None;
    //delete klasses;
    delete heap;
    cout << "Universe destroyed" << endl;
}

void Universe::oops_do(OopClosure* closure) {
    closure->do_oop((Object**)&True);
    closure->do_oop((Object**)&False);
    closure->do_oop((Object**)&None);

    closure->do_oop((Object**)&main_code);
    closure->do_array_list(&klasses);
}

void Universe::gc() {
    heap->gc();
}