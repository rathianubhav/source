#include <symboltable.h>

SymbolTable*
SymbolTable::search(const string& name)
{
    SymbolTable* cur = this;
    while(cur && cur->bindings.count(name) == 0)
        cur = cur->parent;

    return cur;
}

Value
SymbolTable::lookup(const string &name)
{
    SymbolTable* cur = search(name);
    if (cur)
        return cur->bindings[name];
    else {
        cout << "Error: variable '" << name << "' is not defined" << endl;
        exit(5);
    } 
}

void
SymbolTable::bind(const string &name, Value val)
{
    if (bindings.count(name) == 0)
        bindings[name] = val;
    else {
        cout << "Error: variable '" << name << "' already defined" << endl;
        exit(6);
    }
}

void
SymbolTable::rebind(const string &name, Value val)
{
    SymbolTable* f = search(name);
    if (f) 
        f->bindings[name] = val;
    else {
        cout << "Error: variable '" << name << "' is not defined" << endl;
        exit(8);
    }
}

bool
SymbolTable::isdefined(const string &name)
{
    SymbolTable* f = search(name);
    if (f) return true;
    else return false;
}

void
SymbolTable::print()
{
    if (parent) cout << "-- Child --" << endl;
    else cout << "-- Parent --" << endl;

    for(auto a : bindings) {
        cout << a.first << "\t";
        a.second.repr(cout);
        cout << endl;
    }
    cout << "-------------" << endl;
}