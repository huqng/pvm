#ifndef _UNIVERSE_H
#define _UNIVERSE_H

#include "integerObject.h"

class StringTable {
private:
    static StringTable* instance;
    StringTable();
public:
    static StringTable* get_instance();

    StringObject* str_next;
    StringObject* str_mod;
    StringObject* str_init;
};

class Universe {
public:
    static IntegerObject* True;
    static IntegerObject* False;
    static Object* None;

    static void genesis();
    static void destory();
};

#endif
