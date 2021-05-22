#include "pString.h"
#include <cstring>

PString::PString(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);
}

PString::PString(const char* x, const int length) {
    _length = length;
    _value = new char[_length];
    memcpy(_value, x, _length);
}

const char* PString::value() {
    return _value;
}

const int PString::length() {
    return _length;
}