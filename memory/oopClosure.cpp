#include "oopClosure.h"

#include "arrayList.h"
#include "map.h"
#include "object.h"
#include "universe.h"
#include "interpreter.h"
#include "space.h"
#include "stringObject.h"

#include <cassert>
#include <cstring>
#include <iostream>
using namespace std;

//OopClosure::~OopClosure() { }

ScavengeOopClosure::ScavengeOopClosure(Space* from, Space* to, Space* meta) {
    _from = from;
    _to = to;
    _meta = meta;
    _oop_stack = new stack<Object*>();
}

ScavengeOopClosure::~ScavengeOopClosure() {
    delete _oop_stack;
}

Object* ScavengeOopClosure::copy_and_push(Object* obj) {
    char* target = obj->new_address();

    /* whether has been copied */
    if(target != nullptr) {
        return (Object*)target;
    }

    size_t size = obj->size();
    target = (char*)_to->allocate(size);
    memcpy(target, obj, size);
    obj->set_new_address(target);
    _oop_stack->push((Object*)target);

    //if(obj->klass() == StringKlass::get_instance()) {
    //    cout << "PUSH ";
    //    cout << _oop_stack->top() << ": ";
    //    _oop_stack->top()->print();
    //    cout << endl;
    //}
    return (Object*)target;
}

void ScavengeOopClosure::do_oop(Object** obj) {
    if(obj == nullptr || *obj == nullptr)
        return;
    if(!_from->has_obj((char*)*obj)) 
        return;
    (*obj) = copy_and_push(*obj);
}

void ScavengeOopClosure::do_klass(Klass** k) {
    if(k == nullptr || *k == nullptr)
        return;
    (*k)->oops_do(this);
}

void ScavengeOopClosure::do_loopblock(LoopBlock** lb) {
    if(lb == nullptr || *lb == nullptr)
        return;
    if(!_from->has_obj((char*)*lb)) 
        return;
    size_t size = sizeof(LoopBlock);
    char* target = (char*)_to->allocate(size);
    *lb = (LoopBlock*)target;
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass*>** alist) {
    if(alist == nullptr || *alist == nullptr)
        return;
    if(!_from->has_obj((char*)*alist))
        return;

    /* allocate space in 'to' */
    size_t size = sizeof(ArrayList<Klass*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, *alist, size);

    /* relocate */
    (*(char**)alist) = target;

    /* children objects */
    (*alist)->oops_do(this);
}

void ScavengeOopClosure::do_array_list(ObjList** alist) {
    if(alist == nullptr || *alist == nullptr)
        return;
    if(!_from->has_obj((char*)*alist))
        return;

    size_t size = sizeof(ArrayList<Object*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, *alist, size);
    (*(char**)alist) = target;
    (*alist)->oops_do(this);
}

void ScavengeOopClosure::do_array_list(ArrayList<LoopBlock*>** alist) {
    if(alist == nullptr || *alist == nullptr)
        return;
    if(!_from->has_obj((char*)*alist))
        return;
    size_t size = sizeof(ArrayList<LoopBlock*>);
    char* target = (char*)_to->allocate(size);
    memcpy(target, *alist, size);
    (*(char**)alist) = target;
    (*alist)->oops_do(this);    
}

void ScavengeOopClosure::do_map(ObjMap** amap) {
    if(amap == nullptr || *amap == nullptr)
        return;
    if(!_from->has_obj((char*)*amap))
        return;
    size_t size = sizeof(ObjMap);
    char* target = (char*)_to->allocate(size);
    memcpy(target, *amap, size);
    (*(char**)amap) = target;
    (*amap)->oops_do(this);    
}

void ScavengeOopClosure::do_raw_mem(char** mem, int length) {
    if(mem == nullptr || *mem == nullptr)
        return;
    void* target = _to->allocate(length);
    memcpy(target, *mem, length);
    *mem = (char*)target;
}

void ScavengeOopClosure::scavenge() {
    process_roots();
    while(!_oop_stack->empty()) {
        Object* top = _oop_stack->top();
        _oop_stack->pop();

        //if((top)->klass() == StringKlass::get_instance()) {
        //    cout << "POP  " << top << ": "; 
        //    top->print();
        //    cout << endl;
        //    StringObject* so = (StringObject*)top;
        //}

        top->oops_do(this);
    }
}

void ScavengeOopClosure::process_roots() {
    /* add all roots into oops_stack */
    Universe::oops_do(this);
    Interpreter::get_instance()->oops_do(this);
    StringTable::get_instance()->oops_do(this);
}
