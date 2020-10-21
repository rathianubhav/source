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
        auto s = parse_expr_stmt();
        prog->stmts.push_back(move(s));
    }

    // }

    return move(prog);
}


// expr_stmt ::= <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_expr_stmt()
{
    // eat <expr>
    auto e = parse_expr();

    // eat ';'
    expect(token::semicolon);
    eat_token();

    return make_unique<ast::expr_stmt>(move(e));
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

    // eat '+' | '-'
    if (token::is_operator(curtok.get_type())) {
        if (curtok.get_lit() == "+") {
            eat_token();
            auto rval = parse_term();
            auto val = make_unique<ast::constant>((lval->get() + rval.get()->get()));
            return parse_expr_tail(move(val));
        } else if (curtok.get_lit() == "-") {
            eat_token();
            auto rval = parse_term();
            auto val = make_unique<ast::constant>((lval->get() - rval.get()->get()));
            return parse_expr_tail(move(val));
        }
    }

    return move(lval);
}

// term_tail ::= '*' <factor> <term_tail>
//           ::= '/' <factor> <term_tail>
//           ::= <empty>
unique_ptr<ast::expr>
parser::obj::parse_term_tail(unique_ptr<ast::expr> lval)
{
    // eat '+' | '-'
    if (token::is_operator(curtok.get_type())) {
        if (curtok.get_lit() == "*") {
            eat_token();
            auto rval = parse_factor();
            auto val = make_unique<ast::constant>((lval->get() * rval.get()->get()));
            return parse_term_tail(move(val));
        } else if (curtok.get_lit() == "-") {
            eat_token();
            auto rval = parse_factor();
            auto val = make_unique<ast::constant>((lval->get() / rval.get()->get()));
            return parse_term_tail(move(val));
        }
    }

    return move(lval);
}

// factor ::= '(' <expr> ')'
//        ::= NUM
unique_ptr<ast::expr>
parser::obj::parse_factor()
{
    int val = 0;
    if (curtok.get_lit() == "(")
    {
        // eat '('
        eat_token();

        // eat <expr>
        val = parse_expr()->get().int_val();

        // eat ')'
        eat_token();
    } else {
        val = atoi(curtok.get_lit().c_str());
        eat_token();
    }
    return make_unique<ast::num>(val);
}