#ifndef _ARRAY_LIST_H
#define _ARRAY_LIST_H


#include <iostream>
#include <cstring>
using namespace std;


template<typename T>
class ArrayList {
private:
    int _max_size; /* max size of array */
    T* array;
    int _size; 
    void expand(); /* used size of array */
public:
    ArrayList(int n);
    ArrayList();
    ~ArrayList();

    void append(T t);
    void insert(int index, T t);
    void set(int index, T t);
    T get(int index);
    int size();
    int length();
    T pop();
};



template<typename T>
ArrayList<T>::ArrayList(int n) {
    _max_size = n;
    _size = n;
    array = new T[n];
}

template<typename T>
ArrayList<T>::ArrayList() {
    _max_size = 8;
    _size = 0;
    array = new T[8];
}

template<typename T>
ArrayList<T>::~ArrayList() {
    delete[] array;
}

template<typename T>
void ArrayList<T>::append(T t) {
    if(_size >= _max_size)
        expand();
    array[_size++] = t;
}

template<typename T>
void ArrayList<T>::insert(int index, T t) {
    if(index < 0 && index > _size) {
        cerr << "insert: invalid index" << endl;
        exit(-1);
    }
    append(t);
    for(int i = _size - 1; i > index; i--) {
        array[i] = array[i - 1]; 
    }
    array[index] = t;
}

template<typename T>
void ArrayList<T>::expand() {
    T* newarray = new T[_max_size * 2];
    memcpy(newarray, array, sizeof(T) * _max_size);
    delete[] array;
    array = newarray;
    _max_size *= 2;
}

template<typename T>
int ArrayList<T>::size() {
    return _size;
}

template<typename T>
int ArrayList<T>::length() {
    return _max_size;
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
    if(index < 0) {
        cerr << "error arraylist set: index = " << index << endl;
        exit(-1);
    }
    if(_size <= index)
        _size = index + 1;
    while(_max_size < _size) {
        this->expand();
    }
    array[index] = t;
}

template<typename T>
T ArrayList<T>::pop() {
    if(_size > 0)
        return array[--_size];
    else {
        cout << "error pop from empty arraylist" << endl;
        exit(-1);
    }
}

class Object;
typedef ArrayList<Object*> ObjList;

#endif
