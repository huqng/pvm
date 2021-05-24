#ifndef _PVM_MAP_H
#define _PVM_MAP_H

#include "../runtime/universe.h"
#include <iostream>
using namespace std;

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
    V value() { return _v; }
    void set(K k, V v) { _k = k; _v = v; }
};

template<typename K, typename V>
class Map {
private:
    MapEntry<K, V>* _entries;
    int _size;
    int _length;

    /* tmp for array-based implementation */
    void    expand();
public:
    Map();
    ~Map();

    int     size() { return _size; }
    MapEntry<K, V>* entries() { return _entries; }

    void    put(K k, V v);
    V       get(K k);

    K       get_key(int index);
    int     index(K k);
    
    bool    has_key(K k);
    V       remove(K k);
};

template<typename K, typename V>
Map<K, V>::Map() {
    _size = 0;
    _length = 2;
    _entries = new MapEntry<K, V>[8];
}

template<typename K, typename V>
Map<K, V>::~Map() {
    delete[] _entries;
}

template<typename K, typename V>
void Map<K, V>::put(K k, V v) {
    if(_size >= _length) {
        expand();
    }
    int i;
    for(i = 0; i < _size; i++) {
        if(_entries[i].key() == k) {
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
        if(_entries[i].key() == k)
            return _entries[i].value();
    }
    cerr << "error Map::get: invalid key [" << ((PString*)k)->value() << "]" << endl;
    exit(-1);
}

template<typename K, typename V>
K Map<K, V>::get_key(int index) {
    if(index >= _size || index < 0) {
        cerr << "error get_key: invalid index" << endl;
        exit(-1);
    }
    return _entries[index].key();

}

template<typename K, typename V>
void Map<K, V>::expand() {
    MapEntry<K, V>* p = new MapEntry<K, V>[_length * 2];
    memcpy(p, _entries, sizeof(MapEntry<K, V>) * _length);
    delete[] _entries;
    _entries = p;
    _length *= 2;
}

template<typename K, typename V>
bool Map<K, V>::has_key(K k) {
    for(int i = 0; i < _size; i++) {
        if(_entries[i].key() == k)
            return true;
    }
    return false;
}

template<typename K, typename V>
V Map<K, V>::remove(K k) {
    if(has_key(k)) {
        int id = index(k);
        V tmpv = _entries[id];
        _entries[id] = _entries[--_size];
        return tmpv;
    }
    cerr << "error remove: invalid key" << endl;
    exit(-1);
}

#endif
