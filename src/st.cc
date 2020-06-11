#include <st.h>
#include <buffer.h>

extern source::out::stream err;

source::ST*
source::ST::search(std::string name)
{
    ST* cur = this;
    while(cur && cur->bindings.count(name) == 0) cur = cur->parent;
    return cur;
}

source::Value
source::ST::lookup(std::string name)
{
    ST* cur = search(name);
    if (cur) return cur->bindings[name];
    else {
        err << "err[st]: no binding in symboltable for '" << name << "'" << std::endl;
        exit(5);
    }
}


void
source::ST::bind(std::string name, source::Value val)
{
    if (bindings.count(name) == 0) bindings[name] = val;
    else {
        err << "err[st]: variable '" << name << "' already bound!" << std::endl;
        exit(5);
    }
}

void
source::ST::rebind(std::string name, source::Value val)
{
    auto f = search(name);
    if (f) f->bindings[name] = val;
    else {
        err << "err[st]: variable '" << name << "' not already bound!" << std::endl;
        exit(5);
    }
}