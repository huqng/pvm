#include "cellObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"

CellKlass* CellKlass::instance = nullptr;

CellKlass::CellKlass() {
}

CellKlass* CellKlass::get_instance() {
    if(instance == nullptr)
        instance = new CellKlass();
    return instance;
}

void CellKlass::initialize() {
    set_name(new StringObject("Cell"));
    
    /* set type_object */
    TypeObject* obj = new TypeObject(this);
    set_type_object(obj);

    set_super(ObjectKlass::get_instance());
}


CellObject::CellObject(ListObject* table, int index) {
    _table = table;
    _index = index;
    set_klass(CellKlass::get_instance());
}

Object* CellObject::value() {
    return _table->get(_index);
}
