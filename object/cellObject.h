#ifndef _CELL_OBJECT_H
#define _CELL_OBJECT_H

#include "listObject.h"

class CellKlass;
class CellObject;

class CellKlass: public Klass {
private:
    CellKlass();
    static CellKlass* instance;
public:
    static CellKlass* get_instance();
};

class CellObject: public Object {
private:
    ListObject* _table;
    int _index;
public:
    CellObject(ListObject* table, int index);
    Object* value();
};

#endif
