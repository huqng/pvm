#include "heap.h"
#include "space.h"
#include "oopClosure.h"
#include <iostream>
using namespace std;

Heap* Heap::instance = nullptr;
//unsigned int Heap::MAX_CAP =  1 << 21; // 2MB
unsigned int Heap::MAX_CAP =  1 << 17; // 128K

Heap* Heap::get_instance() {
    if(instance == nullptr) {
        instance = new Heap(MAX_CAP);
    }
    return instance;
}

Heap::Heap(unsigned int size) {
    mem_1 = new Space(size);
    mem_2 = new Space(size);
    metaspace = new Space(size);

    mem_1->clear();
    mem_2->clear();
    metaspace->clear();

    eden = mem_1;
    surviver = mem_2;
}

Heap::~Heap() {
    if(mem_1 != nullptr) {
        delete mem_1;
        mem_1 = nullptr;
    }
    if(mem_2 != nullptr) {
        delete mem_2;
        mem_2 = nullptr;
    }
    if(metaspace != nullptr) {
        delete metaspace;
        metaspace = nullptr;
    }
    eden = nullptr;
    surviver = nullptr;
}

void* Heap::allocate(unsigned int size) {
    if(!eden->can_alloc(size))
        gc();
    return eden->allocate(size);
}

void* Heap::allocate_meta(unsigned int size) {
    if(!metaspace->can_alloc(size))
        return nullptr;
    return metaspace->allocate(size);
}

void Heap::copy_live_objects() {
    // TODO - copy live object from eden to surviver, swap eden and surviver, and then clear surviver
    ScavengeOopClosure closure(eden, surviver, metaspace);
    closure.scavenge();
}

void Heap::gc() {
    copy_live_objects();
    Space* t = eden;
    eden = surviver;
    surviver = t;
    surviver->clear();

    cout << endl << "GC" << endl << endl;
}

void Heap::print_info() {
    cout << endl;
    cout << "HEAP: " << endl;
    cout << "EDEN = " << (void*)eden->_base << "-" << (void*)eden->_end << endl;
    cout << "SURV = " << (void*)surviver->_base << "-" << (void*)surviver->_end << endl;
    cout << endl;
}
