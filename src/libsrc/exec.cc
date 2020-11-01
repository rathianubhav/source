#include <source/ast.hh>

using namespace source;

int
ast::var_decl::exec(context::obj& cc)
{
    
    auto var_id = iden->get().str_val();

    if (expr) cc.st.insert(var_id, expr->eval(cc));
    
    else cc.st.insert(var_id, value::obj(type));
    
    debug("inserted variable: ", var_id);
}

int
ast::debug_stmt::exec(context::obj& cc)
{
    switch (dc)
    {
        case 0: cc.st.print();  break;
        default:    cout << "no debug" << endl;
    }
}