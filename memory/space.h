#ifndef _SPACE_H
#define _SPACE_H

class Space {
    char* _base;
    char* _top;
    char* _end;
    unsigned int _size;
    unsigned int _capacity;

    Space(unsigned int size);
    ~Space();
public:
    void* allocate(unsigned int size);
    void clear();
    bool can_alloc(unsigned int size);
    bool has_obj(char* obj);

    friend class Heap;
};

#endif
