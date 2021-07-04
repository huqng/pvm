#ifndef _ARRAY_LIST_H
#define _ARRAY_LIST_H

#include "universe.h"
#include "oopClosure.h"
#include "heap.h"
#include <iostream>
#include <cstring>
using namespace std;

class Object;
class Klass;
class OopClosure;
class loopBlock;

template<typename T>
class ArrayList {
private:
    typedef bool(*eq_t)(T, T);
    eq_t _eq;

    int _max_size; /* max size of array */
    T* array;
    int _size; /* used size of array */
    void expand(); 
public:
    ArrayList(int n, eq_t eq = [](T t1, T t2){ return t1 == t2; });
    ArrayList(eq_t eq = [](T t1, T t2){ return t1 == t2; });
    ~ArrayList();

    void append(T t);
    void insert(int index, T t);
    void set(int index, T t);
    T get(int index);
    int size();
    int length();
    T pop();
    void delete_index(int index);
    int index(T t);
    void oops_do(OopClosure* closure);
    void* operator new(size_t size);
};

template class ArrayList<Object*>;
template class ArrayList<Klass*>;
template class ArrayList<LoopBlock*>;

typedef ArrayList<Object*> ObjList;

template<typename T>
ArrayList<T>::ArrayList(int n, eq_t eq) {
    _max_size = n;
    _size = n;
    _eq = eq;
    void* tmp = Universe::heap->allocate(sizeof(T) * (_max_size * 2));
    array = new(tmp)T[_max_size * 2];
}

template<typename T>
ArrayList<T>::ArrayList(eq_t eq) {
    _max_size = 8;
    _size = 0;
    _eq = eq;
    void* tmp = Universe::heap->allocate(sizeof(T) * (_max_size * 2));
    array = new(tmp)T[_max_size * 2];
}

template<typename T>
ArrayList<T>::~ArrayList() {
    //delete[] array;
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
    void* tmp = Universe::heap->allocate(sizeof(T) * (_max_size * 2));
    T* newarray = new(tmp)T[_max_size * 2];
    memcpy(newarray, array, sizeof(T) * _max_size);
    //delete[] array;
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



template<typename T>
void ArrayList<T>::delete_index(int index) {
    if(index < 0 || index >= _size) {
        cerr << "error arraylist delete index: index = " << index;
        cerr << "size = " << _size << endl;
        exit(-1);
    }
    for(int i = index; i < _size - 1; i++) {
        array[i] = array[i + 1];
    }
    _size -= 1;
}

template<typename T>
int ArrayList<T>::index(T t) {
    for(int i = 0; i < _size; i++) {
        if(_eq(array[i], t)) {
            return i;
        }
    }
    return -1;
}

template<typename T>
void* ArrayList<T>::operator new(size_t size) {
    return Universe::heap->allocate(sizeof(ArrayList<T>));
}

template<>
void ArrayList<Klass*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)&array, _max_size * sizeof(Klass*));
    for(int i = 0; i < _size; i++)
        closure->do_klass((Klass**)&array[i]);
}

template<>
void ArrayList<Object*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)&array, _max_size * sizeof(Object*));
    for(int i = 0; i < _size; i++)
        closure->do_oop((Object**)&array[i]);
}

template<>
void ArrayList<LoopBlock*>::oops_do(OopClosure* closure) {
    closure->do_raw_mem((char**)&array, _max_size * sizeof(LoopBlock*));
    for(int i = 0; i < _size; i++)
        closure->do_loopblock((LoopBlock**)&array[i]);
}

#endif
