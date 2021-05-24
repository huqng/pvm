#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "../code/codeObjet.h"
#include "../utils/map.h"

class CodeObject;

class Interpreter {
private:
    /* initialized in run() */
    ArrayList<PObject*>* _stack;
    ArrayList<PObject*>* _consts;
    ArrayList<PObject*>* _names;

    Map<PString*, PObject*>* _vars;


    void push(PObject* p);
    PObject* pop();
public:
    void run(CodeObject* co);
};

#endif
