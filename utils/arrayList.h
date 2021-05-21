#ifndef _ARRAY_LIST_H
#define _ARRAY_LIST_H

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

#endif
