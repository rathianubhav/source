#include <source/parser.hh>
#include <memory>

using namespace source;


std::map<std::string,value::type> data_types = {
    {"int",value::int_t},
};


bool is_datatype(token::obj &t)
{
    return data_types.find(t.get_lit()) != data_types.end();
}

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
        unique_ptr<ast::stmt> s;

        debug("current token: ",curtok.get_lit());
        
        if (is_datatype(curtok)){
            io::info("parsing root decl");
            s = parse_root_declaration();
        }   
        else if (curtok.get_type() == token::__debug)
            s = parse_debug_stmt();
        else                        s = parse_expr_stmt();
        prog->stmts.push_back(move(s));
    }

    // }

    return move(prog);
}


// debug_stmt   ::= 'debug' '<?>' 
unique_ptr<ast::stmt>
parser::obj::parse_debug_stmt()
{
    debug("parsing debug statment");
    // eat '#debug'
    eat_token();
    int s = -1;
    if (curtok.get_lit() == "st")
        s = 0;

    return make_unique<ast::debug_stmt>(s);
}

// root_delaration  ::= var_decl | func_decl
// var_decl         ::= TYPE {'*'}* IDEN ';'
//                  ::= TYPE {'*'}* IDEN  '=' <expr> ';'
// func_decl        ::= TYPE {'*'}* IDEN '(' param_decl ')' '{' body_decl '}'
unique_ptr<ast::stmt>
parser::obj::parse_root_declaration()
{
    debug("parsing root declaration statment");
    // eat TYPE
    auto type = data_types[curtok.get_lit()];
    eat_token();

    // {'*'}*
    int ptr = -1;
    while(curtok.get_type() == token::star)
    {
        eat_token();
        ptr++;
    }

    // eat IDEN
    expect(token::ident);
    auto iden = std::make_unique<ast::identifier>(curtok.get_lit());
    eat_token();

    if (curtok.get_type() == token::semicolon)
    {
        // eat ';'
        eat_token();
        return make_unique<ast::var_decl>(type, move(iden), ptr);
    }

    else if (curtok.get_type() == token::assign)
    {
        // eat '='
        eat_token();
        auto e = parse_expr();

        // eat ';'
        eat_token();
        return make_unique<ast::var_decl>(type, move(iden), ptr, move(e));
    } 
    return nullptr;
    
}

// expr_stmt ::= <expr> ';'
unique_ptr<ast::stmt>
parser::obj::parse_expr_stmt()
{
    debug("parsing expr_stmt");
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
        } else if (curtok.get_lit() == "/") {
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
//        ::= IDEN
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