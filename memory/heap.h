#ifndef _HEAP_H
#define _HEAP_H

class Space;

class Heap {
    Space* mem_1;
    Space* mem_2;
    Space* eden;
    Space* surviver;
    Space* metaspace;
    Heap(unsigned int size);
public:
    static unsigned int MAX_CAP;
    static Heap* instance;
    static Heap* get_instance();
    ~Heap();

    void* allocate(unsigned int size);
    void* allocate_meta(unsigned int size);
    void copy_live_objects();

    void gc();

    void print_info();
};

#endif
