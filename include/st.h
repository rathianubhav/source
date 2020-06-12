#pragma once

#include "value.h"
#include <map>
#include <string>

namespace source {

class ST {
private:
    
    ST* parent;

    ST* search(std::string name);

public:
    std::map<std::string, Value> bindings;
    ST(ST*par) : parent(par) {}
    Value lookup(std::string name);
    void bind(std::string, Value);
    void rebind(std::string, Value);

    bool defined(std::string);
};

}