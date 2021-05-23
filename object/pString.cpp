#include "pString.h"
#include <cstring>
#include <iostream>
using namespace std;

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

void PString::print() {
    cout << this->value();
}

PObject* PString::add(PObject* x) {
    // TODO - strcat
    return this;
}
