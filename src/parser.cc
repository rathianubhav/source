#include <source/parser.hh>
#include <memory>

using namespace source;

void
parser::obj::eat_token()
{
    curtok = peektok;
    peektok = __lexer__.next_token();
}

void
parser::obj::expect(token::type t)
{
    if (t != curtok.get_type()) {
        __lexer__.throw_error("unexpected token of type '" + token::tok_to_str(curtok.get_type()) + "' but required '" + token::tok_to_str(t)+ "'") ;
        throw runtime_error("unexpected token");
    }
}

// prog ::= { <stmt> }
unique_ptr<ast::program> 
parser::obj::parse()
{
    auto prog = make_unique<ast::program>();

    // {
    while (curtok.get_type() != token::eof) {

        // eat <stmt>
        auto s = parse_stmt();
        if (s != nullptr) {
            prog->stmts.push_back(move(s));
        }
        eat_token();
    }

    // }

    return move(prog);
}
