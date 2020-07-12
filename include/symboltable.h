#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

#include "value.h"
#include <map>
#include <string>

class SymbolTable {
private:
    SymbolTable* parent;
    SymbolTable* search(const string&);

public:
    map<string,Value> bindings;
    
    SymbolTable(SymbolTable *p) : parent(p) {}

    Value lookup(const string&);
    void bind(const string&, Value);
    void rebind(const string&, Value);

    bool isdefined(const string&);
    void print();
};

#endif