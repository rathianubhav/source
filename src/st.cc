#include <source/st.hh>

using namespace source;

void
st::obj::insert(const string &s, value::obj v)
{
    if (__table__.find(s) != __table__.end())
        throw runtime_error("variable '" + s + "' is already initialized");
    
    __table__[s] = v;
}

void
st::obj::update(const string &s, value::obj v)
{
    if (__table__.find(s) == __table__.end())
        throw runtime_error("variable '" + s + "' is not yet initialized");
    

    if (__table__[s].get_type() != v.get_type())
        throw runtime_error("can't assign variable of different type");
    
    __table__[s] = v;
}

value::obj
st::obj::lookup(const string &s)
{
    if (__table__.find(s) == __table__.end())
        throw runtime_error("variable '" + s + "' is not yet initialized");
    
    return __table__[s];
}

void
st::obj::print()
{
    cout << "----------------------------" << endl;
    cout << " Variable\t|\tValue     " << endl;
    for(auto a : __table__)
    {
        cout << " " << a.first << "\t\t|\t\t" << a.second << endl;
    }
    cout << "----------------------------" << endl;
}