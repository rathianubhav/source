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

// prog ::= [ <root_declaration> ]
unique_ptr<ast::program> 
parser::obj::parse()
{
    auto prog = make_unique<ast::program>();

    // {
    while (curtok.get_type() != token::eof) {

    }

    // }

    return move(prog);
}


// expr ::= <term> <expr_tail>
unique_ptr<ast::expr>
parser::obj::parse_expr()
{
    // eat <term>
    auto lval = parse_term();

    // eat <expr_tail>
    return parse_expr_tail(move(lval));
}

// term ::= <factor> <term_tail>
unique_ptr<ast::expr>
parser::obj::parse_term()
{
    // eat <factor>
    auto lval = parse_factor();

    // eat <term_tail>
    return parse_term_tail(move(lval));
}

// expr_tail ::= '+' <term> <expr_tail>
//            |  '-' <term> <expr_tail>
//            |  empty
unique_ptr<ast::expr>
parser::obj::parse_expr_tail(unique_ptr<ast::expr> lval)
{
    // eat '+'

}