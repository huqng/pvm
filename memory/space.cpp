#include "space.h"

#include <cstring>

Space::Space(unsigned int size) {
    _size = size;
    _base = new char[size];
    _end = _base + size;
    _top = (char*)(((long int)(_base + 0xFFFF)) & -16);
    _capacity = _end - _top;
}

Space::~Space() {
    if(_base != nullptr) {
        delete[] _base;
        _base = nullptr;
    }
    _top = 0;
    _end = 0;
    _capacity = 0;
    _size = 0;
}

void Space::clear() {
    memset(_base, 0, _size);
    _top = (char*)(((long int)(_base + 0xFFFF)) & -16);
    _capacity = _end - _top;
}

void* Space::allocate(unsigned int size) {
    size = (size + 7) & -8;
    char* start = _top;
    _top += size;
    _capacity -= size;
    return start;
}

bool Space::can_alloc(unsigned int size) {
    return _capacity >= size;
}

bool Space::has_obj(char* obj) {
    return obj >= _base && obj < _end;
}
