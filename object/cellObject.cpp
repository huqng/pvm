#include "cellObject.h"

CellKlass* CellKlass::instance = nullptr;

CellKlass::CellKlass() {
    set_name("Cell");
}

CellKlass* CellKlass::get_instance() {
    if(instance == nullptr)
        instance = new CellKlass();
    return instance;
}

CellObject::CellObject(ListObject* table, int index) {
    _table = table;
    _index = index;
    set_klass(CellKlass::get_instance());
}

Object* CellObject::value() {
    return _table->get(_index);
}
