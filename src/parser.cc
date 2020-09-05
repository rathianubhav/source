#include <source/parser.hh>
#include <releax/releax>

using namespace source;
using namespace releax;

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

// stmt ::= <let>
//      ::= <expr_stmt>
//      ::= <print_stmt>
//      ::= <if_stmt>
//      ::= <while_stmt>
unique_ptr<ast::stmt>
parser::obj::parse_stmt()
{
    switch(curtok.get_type()) {
        case token::__let:
            return parse_let();

        case token::__print:
            return parse_print_stmt();

        case token::__if:
            return parse_if_stmt();

        case token::__while:
            return parse_while_stmt();
        
        default:

            if (curtok.get_type() == token::ident &&
                peektok.get_type() == token::assign) 
                return parse_assign_stmt();

            return parse_expr_stmt();
    }

    return make_unique<ast::nullstmt>();
}


// assign_stmt ::= <id> '=' <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_assign_stmt()
{
    // eat <id>
    auto id = make_unique<ast::ident>(curtok.get_lit());
    eat_token();

    // eat '='
    expect(token::assign);
    eat_token();

    // eat <expr>
    auto e = parse_expr();

    // eat ';'
    expect(token::semicolon);

    return make_unique<ast::assign>(move(id), move(e));
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
        case value::STRING:
            cout << "str()" << endl;
            break;

        default:
            cout << "invalid()" << endl;
            break;
    }

    return make_unique<ast::nullstmt>();
}

// let ::= 'let' <iden> '=' <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_let()
{
    // eat 'let'
    eat_token();

    // eat <ident>
    expect(token::ident);
    auto id = make_unique<ast::ident>(curtok.get_lit());
    eat_token();

    // eat '='
    expect(token::assign);
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

// if_stmt ::= 'if' '(' <expr> ')' '{' <stmt> '}'
unique_ptr<ast::stmt>
parser::obj::parse_if_stmt()
{
    // eat 'if'
    eat_token();

    // eat '('
    eat_token();


    // eat '{'
    eat_token();

    // eat <expr>
    auto e = parse_expr();

    // eat ')'
    eat_token();

    // eat '{'
    eat_token();

    // eat <stmt>
    auto s = parse_stmt();
    
    // eat '}'
    eat_token();

    return make_unique<ast::condition>(move(e),move(s));
}

// print_stmt ::= 'print' <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_print_stmt()
{
    // eat 'print'
    eat_token();

    // eat <expr>
    auto e = parse_expr();
    if (e == nullptr) {
        __lexer__.throw_error("expected expression on print_statment()");
        return nullptr;
    }
    eat_token();

    return make_unique<ast::print_stmt>(move(e));
}


// ident ::= <id>
//       ::= <id> '(' ')'
unique_ptr<ast::expr>
parser::obj::parse_ident()
{
    // eat <id>
    auto id = make_unique<ast::ident> (curtok.get_lit());
    eat_token();

    if (curtok.get_type() != token::lparen) {
        return move(id);
    }

    // '('
    expect(token::lparen);
    eat_token();


    // ')'
    expect(token::rparen);
    eat_token();


    return make_unique<ast::call> (move(id));
}

// func ::= 'func' '(' ')' '{' stmt '}'
unique_ptr<ast::expr>
parser::obj::parse_func()
{
    // eat 'func'
    eat_token();

    // eat '('
    expect(token::lparen);
    eat_token();

    // eat ')'
    expect(token::rparen);
    eat_token();

    // eat '{'
    expect(token::lbrace);
    eat_token();

    // eat <stmt>
    auto s = parse_stmt();

    // eat '}'
    expect(token::rbrace);
    eat_token();


    return make_unique<ast::method>(s.get());
}

// expr ::= <bin_expr>
//      ::= <num>
//      ::= <str>
//      ::= <iden>
//      ::= <func>
unique_ptr<ast::expr>
parser::obj::parse_expr()
{
    if (token::is_operator(peektok.get_type()))
        return parse_op_expr();

    switch (curtok.get_type()) {
        case token::__int:
            return move(parse_num());

        case token::ident:
            return move(parse_ident());

        case token::__string:
            return move(parse_str());
        
        case token::__func:
            return move(parse_func());
    }

    return nullptr;
}

// bin_expr  ::= <expr> OP <expr>
unique_ptr<ast::expr>
parser::obj::parse_op_expr()
{

    // eat <expr>
    auto l = parse_expr();

    
    // eat OP
    auto oper = curtok.get_type();
    eat_token();

    // eat <expr>
    auto r = parse_expr();

    return make_unique<ast::binexpr>(move(l), oper, move(r));
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

// str ::= STRING
unique_ptr<ast::expr>
parser::obj::parse_str()
{
    // eat STRING
    auto s = make_unique<ast::str>(curtok.get_lit());
    eat_token();

    return move(s);
}

/// while_stmt ::= 'while' '(' <expr> ')' '{' <stmt> '}'
///            ::= 'while' '(' <expr> ')' <stmt>
unique_ptr<ast::stmt>
parser::obj::parse_while_stmt()
{
    // eat 'while'
    eat_token();

    // eat '('
    eat_token();

    // <expr>
    auto ex = parse_expr();
    eat_token();

    // ')'
    eat_token();

    // '{' or <expr>
    if (curtok.get_type() == token::lbrace)
        eat_token();

    // <stmt>
    auto s = parse_stmt();
    eat_token();

    // '}'
    if (curtok.get_type() == token::rbrace)
        eat_token();


    auto __wh = make_unique<ast::while_loop>(move(ex), move(s));
    return move(__wh);
}
