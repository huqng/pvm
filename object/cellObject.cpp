#include "cellObject.h"
#include "listObject.h"
#include "stringObject.h"
#include "typeObject.h"
#include "oopClosure.h"

#include <cassert>

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

    add_super(ObjectKlass::get_instance());
}

size_t CellKlass::size() {
    return sizeof(CellObject);
}

void CellKlass::oops_do(OopClosure* closure, Object* obj) {
    assert(obj->klass() == this);
    CellObject* cobj = (CellObject*)obj;
    closure->do_oop(cobj->table_address());
}


CellObject::CellObject(ListObject* table, int index) {
    _table = table;
    _index = index;
    set_klass(CellKlass::get_instance());
}

Object* CellObject::value() {
    return _table->get(_index);
}

Object** CellObject::table_address() {
    return (Object**)&_table;
}