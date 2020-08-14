#include <source/st.hh>

using namespace source;

void
st::obj::insert(const string &s, value::obj v)
{
    if (__table__.find(s) != __table__.end())
        throw runtime_error("variable '" + s + "' is already initialized");
    
    __table__[s] = v;
}


value::obj
st::obj::lookup(const string &s)
{
    if (__table__.find(s) == __table__.end())
        throw runtime_error("variable '" + s + "' is not yet initialized");
    
    return __table__[s];
}