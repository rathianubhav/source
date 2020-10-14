#include <source/ast.hh>

using namespace source::ast;

string
program::label()
{
    string ret = "";
    for(auto i = stmts.begin(); i != stmts.end(); i++)
        ret += i->get()->label() + " ";
    
    return ret;
}

string 
let::label()
{
    return "let(" +
            id->label() + ":" +
            val->label() + ")";
}

string 
assign::label()
{
    return "assign(" +
            id->label() + ":" +
            val->label() + ")";
}

string
binexpr::label()
{
    return "bin(" +
            left->label() + token::tok_to_str(oper) +
            right->label() + ")";
}