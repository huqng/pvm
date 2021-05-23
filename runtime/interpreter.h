#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../code/codeObjet.h"

class CodeObject;

class Interpreter {
private:
    ArrayList<PObject*>* _stack;
    ArrayList<PObject*>* _consts;

    void push(PObject* p);
    PObject* pop();
public:
    void run(CodeObject* co);
};

#endif
