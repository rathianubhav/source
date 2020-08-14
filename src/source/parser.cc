#include <source/parser.hh>

using namespace source;

void
parser::obj::eat_token()
{
    curtok = peektok;
    peektok = __lexer__->next_token();
}

void
parser::obj::expect(token::type t)
{
    if (t != curtok.get_type()) {
        throw runtime_error("unexpected token parser->expect()");
    }
}

// prog ::= { <stmt> }
unique_ptr<ast::program> 
parser::obj::parse()
{
    auto prog = make_unique<ast::program>();

    // {
    while (curtok.get_type() != token::TEOF) {

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

// stmt ::= <let>
//      ::= <expr_stmt>
unique_ptr<ast::stmt>
parser::obj::parse_stmt()
{
    switch(curtok.get_type()) {
        case token::LET:
            return parse_let();
        
        default:
            return parse_expr_stmt();
    }

    return nullptr;
}

// expr_stmt ::= <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_expr_stmt()
{
    // eat <expr>
    auto e = parse_expr();

    // eat ';'
    eat_token();

    auto val = e->eval(__context__);
    switch(val.get_type()) {
        case value::INT: 
            cout << "num()" << endl;
            break;
        
        default:
            cout << "invalid()" << endl;
            break;
    }

    return nullptr;
}

// let ::= 'let' <iden> '=' <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_let()
{
    // eat 'let'
    eat_token();

    // eat <ident>
    expect(token::IDENT);
    auto id = make_unique<ast::ident>(curtok.get_lit());
    eat_token();

    // eat '='
    expect(token::ASSIGN);
    eat_token();

    // eat expr
    auto e = parse_expr();
    if (e == nullptr) throw runtime_error("unexpected error");
    eat_token();

    // eat ';'
    eat_token();

    auto s = make_unique<ast::let>(move(id), move(e));

    return move(s);
}


// ident ::= <id> ';'
unique_ptr<ast::expr>
parser::obj::parse_ident()
{
    // eat <id>
    auto id = make_unique<ast::ident> (curtok.get_lit());
    eat_token();

    expect(token::SEMICOLON);
    eat_token();

    return make_unique<ast::call> (move(id));
}

// expr ::= <num>
unique_ptr<ast::expr>
parser::obj::parse_expr()
{
    switch (curtok.get_type()) {
        case token::INT:
            return move(parse_num());
        case token::IDENT:
            return move(parse_ident());
    }

    return nullptr;
}

// num ::= INT
unique_ptr<ast::expr>
parser::obj::parse_num()
{
    // eat INT
    int num = atoi(curtok.get_lit().c_str());
    auto n = make_unique<ast::num>(num);
    eat_token();

    return move(n);
}