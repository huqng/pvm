#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "heap.h"

template<typename T> class ArrayList;

class StringObject;
class IntegerObject;
class Object;
class Klass;

class StringTable {
private:
    static StringTable* instance;
    StringTable();
public:
    static StringTable* get_instance();

    StringObject* str_next;
    StringObject* str_mod;
    StringObject* str_init;

    /* operators */
    StringObject* str_add;
    StringObject* str_sub;
    StringObject* str_mul;
    StringObject* str_div;
    StringObject* str_neg;
    StringObject* str_lt;
    StringObject* str_gt;
    StringObject* str_le;
    StringObject* str_ge;
    StringObject* str_eq;
    StringObject* str_ne;
    StringObject* str_and;
    StringObject* str_or;
    StringObject* str_invert;
    StringObject* str_xor;
    StringObject* str_len;
    StringObject* str_call;
    StringObject* str_getattr;
    StringObject* str_getitem;
};

class Universe {
public:
    static IntegerObject* True;
    static IntegerObject* False;
    static Object* None;
    static Heap* heap;
    static ArrayList<Klass*>* klasses;

    static void genesis();
    static void destory();
};

#endif
