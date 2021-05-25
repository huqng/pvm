#ifndef _CLASS_H
#define _CLASS_H

class StringObject;
class PObject;

class Klass {
private:
    StringObject* _name;
public:
    Klass() {}
    void            set_name(StringObject* x) { _name = x; }
    StringObject*    name() { return _name; }

    virtual void print(PObject* obj) {}

    virtual PObject* greater(PObject* x, PObject* y) { return nullptr; }
    virtual PObject* less   (PObject* x, PObject* y) { return nullptr; }
    virtual PObject* eq     (PObject* x, PObject* y) { return nullptr; }
    virtual PObject* ne     (PObject* x, PObject* y) { return nullptr; }
    virtual PObject* ge     (PObject* x, PObject* y) { return nullptr; }
    virtual PObject* le     (PObject* x, PObject* y) { return nullptr; }

    virtual PObject* add(PObject* x, PObject* y) { return nullptr; }
    virtual PObject* sub(PObject* x, PObject* y) { return nullptr; }
    virtual PObject* mul(PObject* x, PObject* y) { return nullptr; }
    virtual PObject* div(PObject* x, PObject* y) { return nullptr; }
    virtual PObject* mod(PObject* x, PObject* y) { return nullptr; }
};

#endif