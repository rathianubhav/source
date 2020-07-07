#ifndef __CONTEXT__
#define __CONTEXT__

#include "symboltable.h"

class Context {
public:
    int loop = 0;
    int func = 0;
    SymbolTable st;

    Context() : st(SymbolTable(nullptr)) {}
    Context(SymbolTable st) : st(st) {}
};

#endif