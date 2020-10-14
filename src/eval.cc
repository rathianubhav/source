#include <source/ast.hh>

using namespace source;
using namespace source::ast;

value::obj
ast::ident::eval(context::obj& cc)
{
    return cc.st.lookup(val);
}

value::obj
ast::call::eval(context::obj& cc)
{
    value::clouser fn = id->eval(cc).get_func();
    if (fn.lambda == nullptr) {
        cout << "call::eval()" << endl;
        return value::obj();
    }

    context::obj fcc;
    fn.lambda->get_body()->exec(fcc);
    return value::obj();
}

value::obj
ast::binexpr::eval(context::obj& cc)
{
    int l = left->eval(cc).get_int();
    int r = right->eval(cc).get_int();

    switch (oper) {
        case token::plus:
            return value::obj(l + r);
        
        case token::minus:
            return value::obj(l - r);
        
        case token::slash:
            return value::obj(l / r);
        
        case token::star:
            return value::obj(l * r);
    }

    return value::obj(false);
}