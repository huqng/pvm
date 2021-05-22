#ifndef _ARRAY_LIST_H
#define _ARRAY_LIST_H


#include <iostream>
#include <cstring>
using namespace std;

template<typename T>
class ArrayList {
private:
    int _length;
    T* array;
    int _size;
    void expand();
public:
    ArrayList(int n = 8);
    ~ArrayList();

    void add(T t);
    void insert(int index, T t);
    void set(int index, T t);
    T get(int index);
    int size();
    int length();
    T pop();
};



template<typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _size = 0;
    array = new T[n];
}

template<typename T>
ArrayList<T>::~ArrayList() {
    delete[] array;
}

template<typename T>
void ArrayList<T>::add(T t) {
    if(_size >= _length)
        expand();
    array[_size++] = t;
}

template<typename T>
void ArrayList<T>::insert(int index, T t) {
    if(index < 0 && index > _size) {
        cerr << "insert: invalid index" << endl;
        exit(-1);
    }
    add(t);
    for(int i = _size; i > index; i--) {
        array[i] = array[i - 1]; 
    }
    array[index] = t;
}

template<typename T>
void ArrayList<T>::expand() {
    T* newarray = new T[_length * 2];
    memcpy(newarray, array, sizeof(T) * _length);
    delete[] array;
    array = newarray;
    _length *= 2;
}

template<typename T>
int ArrayList<T>::size() {
    return _size;
}

template<typename T>
int ArrayList<T>::length() {
    return _length;
}

template<typename T>
T ArrayList<T>::get(int index) {
    if(index < 0 && index >= _size) {
        cerr << "get: invalid index" << endl;
        exit(-1);
    }
    return array[index];
}

template<typename T>
void ArrayList<T>::set(int index, T t) {
    if(index < 0 && index >= _size) {
        cerr << "get: invalid index" << endl;
        exit(-1);
    }
    array[index] = t;
}

template<typename T>
T ArrayList<T>::pop() {
    return array[--size];
}


#endif
