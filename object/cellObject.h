#ifndef _CELL_OBJECT_H
#define _CELL_OBJECT_H

#include "klass.h"
#include "object.h"

class ListObject;
class OopClosure;

class CellKlass;
class CellObject;

class CellKlass: public Klass {
private:
    CellKlass();
    static CellKlass* instance;
public:
    static CellKlass* get_instance();
    void initialize();
    virtual size_t size();
    virtual void oops_do(OopClosure* closure, Object* obj);
};

class CellObject: public Object {
private:
    ListObject* _table;
    int _index;
public:
    CellObject(ListObject* table, int index);
    Object* value();
    Object** table_address();
};

#endif
