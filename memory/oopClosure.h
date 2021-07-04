#ifndef _OOP_CLOSURE_H
#define _OOP_CLOSURE_H

#include <stack>
using namespace std;

class Klass;
class Object;
class Space;
class LoopBlock;
template<typename T>class ArrayList;
template<typename K, typename V>class Map;

class OopClosure {
public:
    virtual ~OopClosure() {}
    virtual void do_oop(Object** obj) = 0;
    virtual void do_loopblock(LoopBlock** lb) = 0;
    virtual void do_array_list(ArrayList<Klass*>** alist) = 0;
    virtual void do_array_list(ArrayList<Object*>** alist) = 0;
    virtual void do_array_list(ArrayList<LoopBlock*>** alist) = 0;
    virtual void do_map(Map<Object*, Object*>** amp) = 0;
    virtual void do_raw_mem(char** mem, int length) = 0;
    virtual void do_klass(Klass** k) = 0;
};

class ScavengeOopClosure: public OopClosure {
private:
    Space* _from;
    Space* _to;
    Space* _meta;
    stack<Object*>* _oop_stack;
    Object* copy_and_push(Object* obj);
public:
    ScavengeOopClosure(Space* from, Space* to, Space* meta);
    virtual ~ScavengeOopClosure();

    virtual void do_oop(Object** obj);
    virtual void do_klass(Klass** k);
    virtual void do_loopblock(LoopBlock** lb);

    virtual void do_array_list(ArrayList<Klass*>** alist);
    virtual void do_array_list(ArrayList<Object*>** alist);
    virtual void do_array_list(ArrayList<LoopBlock*>** alist);
    virtual void do_map(Map<Object*, Object*>** amap);
    virtual void do_raw_mem(char** mem, int length);

    void scavenge();
    void process_roots();
};

#endif
