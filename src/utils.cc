#include <utils.h>


bool 
isInt(const string& num)
{
    for (auto a : num) if (!isdigit(a)) return false; 
    return true;
}

bool 
isFloat(const string& num)
{
    for (auto a : num) if(!(isdigit(a) || a == '.')) return false;
    return true;
}

