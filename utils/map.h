#ifndef _PVM_MAP_H
#define _PVM_MAP_H

#include "universe.h"
#include "oopClosure.h"
#include "heap.h"

#include <iostream>
using namespace std;

class OopClosure;

/* Map Entry */

template<typename K, typename V>
class MapEntry {
private:
    K _k;
    V _v;
public:
    MapEntry(const MapEntry<K, V>& entry);
    MapEntry(K k, V v);
    MapEntry() {}

    K key() { return _k; }
    K* key_addr() { return &_k; }
    V value() { return _v; }
    V* value_addr() { return &_v; }
    void set(K k, V v);
    void* operator new(size_t size);

};

template<typename K, typename V>
void MapEntry<K, V>::set(K k, V v) {
    _k = k;
    _v = v;
}

template<typename K, typename V>
void* MapEntry<K, V>::operator new(size_t size) {
    return Universe::heap->allocate(sizeof(MapEntry<K, V>));
}


/* Map */

template<typename K, typename V>
class Map {
public:
    typedef bool(*eq_t)(K, K);
private:
    MapEntry<K, V>* _entries;
    int             _size;
    int             _max_size;
    eq_t _eq;

    /* tmp for array-based implementation */
    void            expand();
public:
    Map(eq_t eq = [](K t1, K t2) { return t1 == t2; });
    ~Map();

    int             size()      { return _size; }
    MapEntry<K, V>* entries()   { return _entries; }

    void            put(K k, V v);
    V               get(K k);

    K               get_key(int index);
    int             index(K k);
    
    bool            has_key(K k);
    V               remove(K k);

    void*           operator new(size_t size);
    void            oops_do(OopClosure* closure);
};

class Object;
typedef Map<Object*, Object*> ObjMap;

template<typename K, typename V>
void* Map<K, V>::operator new(size_t size) {
    return Universe::heap->allocate(sizeof(Map<K, V>));
}

template<typename K, typename V>
Map<K, V>::Map(eq_t eq) {
    _size = 0;
    _max_size = 8;
    _entries = new MapEntry<K, V>[8];
    _eq = eq;
}

template<typename K, typename V>
Map<K, V>::~Map() {
    
}

template<typename K, typename V>
void Map<K, V>::put(K k, V v) {
    if(_size >= _max_size) {
        expand();
    }
    int i;
    for(i = 0; i < _size; i++) {
        if(_eq(_entries[i].key(), k)) {
            _entries[i].set(k, v);
            break;
        }
    }
    if(i == _size) {
        _entries[_size++].set(k, v);
    }
}

template<typename K, typename V>
V Map<K, V>::get(K k) {
    for(int i = 0; i < _size; i++) {
        if(_eq(_entries[i].key(), k))
            return _entries[i].value();
    }
    // TODO - need special return value to present k is not in map
    return nullptr;
}

template<typename K, typename V>
K Map<K, V>::get_key(int index) {
    if(index >= _size || index < 0) {
        cerr << "error get_key: invalid index" << endl;
        cerr << "size: " << _size << endl;
        exit(-1);
    }
    return _entries[index].key();
}

template<typename K, typename V>
int Map<K, V>::index(K k) {
    for(int i = 0; i < _size; i++) {
        if(_eq(_entries[i].key(), k))
            return i;
    }
    return -1;
}

template<typename K, typename V>
void Map<K, V>::expand() {
    void* tmp = Universe::heap->allocate(sizeof(MapEntry<K, V>) * _max_size * 2);
    MapEntry<K, V>* new_entries = new(tmp)MapEntry<K, V>[_max_size * 2];
    memcpy(new_entries, _entries, sizeof(MapEntry<K, V>) * _max_size);
    _entries = new_entries;
    _max_size *= 2;
}

template<typename K, typename V>
bool Map<K, V>::has_key(K k) {
    for(int i = 0; i < _size; i++) {
        if(_eq(_entries[i].key(), k))
            return true;
    }
    return false;
}

template<typename K, typename V>
V Map<K, V>::remove(K k) {
    if(has_key(k)) {
        int id = index(k);
        V tmpv = _entries[id].key();
        _entries[id] = _entries[--_size];
        return tmpv;
    }
    cerr << "error remove: invalid key" << endl;
    cerr << "size: " << _size << endl;
    exit(-1);
}

template class Map<Object*, Object*>;

template<>
void Map<Object*, Object*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)&_entries, _max_size * sizeof(MapEntry<Object*, Object*>));
    for(int i = 0; i < _size; i++) {
        closure->do_oop(_entries[i].key_addr());
        closure->do_oop(_entries[i].value_addr());
    }
}

#endif
