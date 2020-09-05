#include <source/ast.hh>

using namespace source;
using namespace source::ast;

void
ast::print_stmt::
exec(context::obj& cc)
{
    auto v = e->eval(cc);
    switch (v.get_type()) {
        case value::INT:
            cout << v.get_int() << endl;
            break;
        
        case value::STRING:
            cout << v.get_str() << endl;
            break;
        
        case value::BOOL:
            cout << (v.get_bool()? "true":"false") << endl;
            break;
        
        default:
            cout << "none" << endl;
    }
}


void
ast::condition::exec(context::obj& cc)
{
    // Code 
    if (e->eval(cc).get_bool())
        s->exec(cc);
    
}