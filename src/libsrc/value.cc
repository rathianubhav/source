#include <source/value.hh>

using namespace source;

ostream& value::operator<<(ostream& os, const value::obj& v)
{
    os << v.int_val();
    return os;
}