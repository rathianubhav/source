#include <source/lexer.hh>

using namespace source;

map<string, token::type> __keywords__ {
    {"func", token::FUNC},
    {"let", token::LET},
};

token::type
token::lookup_iden(const string &iden)
{
    if (__keywords__.find(iden) != __keywords__.end())
        return __keywords__[iden];
    
    return token::IDENT;
}

string
token::tok_to_str(token::type t)
{
    switch (t) {
        case token::ILLEGAL: return "illegal";
        case token::TEOF: return "teof";
        case token::IDENT: return "ident";
        case token::INT: return "int";
        case token::ASSIGN: return "assign";
        case token::PLUS: return "plus";
    }

    return "illegal";
}


void
lexer::obj::read_char()
{
    if (read_pos >= input.size())
        ch = 0;
    else
        ch = input[read_pos];
    
    pos = read_pos;
    read_pos++;
    
    col = read_pos;
}

bool
lexer::obj::is_iden(char c)
{
    return 'a' <= c && c <= 'z' ||
            'A' <= c && c <= 'Z' ||
            c == '_';
}

void
lexer::obj::skip_spaces()
{
    while (ch == ' '  ||
           ch == '\t' ||
           ch == '\n' ||
           ch == '\r')
    {
        if (ch == '\n') {
            line++;
            col = 0;
        }
    }
        
        read_char();
}

char
lexer::obj::peek_char()
{
    if (read_pos >= input.size())   return 0;
    return input[read_pos];
}


string
lexer::obj::read_iden()
{
    int p = pos;
    while(is_iden(ch))  read_char();
    return input.substr(p, pos - p);
}

string
lexer::obj::read_num()
{
    int p = pos;
    while(isdigit(ch))  read_char();
    return input.substr(p, pos - p);
}

token::obj
lexer::obj::next_token()
{
    token::obj tok;

    skip_spaces();

    switch (ch) {
        case '=' :
            if (peek_char() == '=') {
                read_char();
                tok = token::obj(token::EQ, "==");
            } else {
                tok = token::obj(token::ASSIGN, ch);
            }
            break;

        case '!':
            if (peek_char() == '=') {
                read_char();
                tok = token::obj(token::NE, "!=");
            } else {
                tok = token::obj(token::BANG, ch);
            }
        
        case ';':
            tok = token::obj(token::SEMICOLON, ch);
            break;
        
        case '(':
            tok = token::obj(token::LPAREN, ch);
            break;

        case ')':
            tok = token::obj(token::RPAREN, ch);
            break;

        case ',':
            tok = token::obj(token::COMMA, ch);
            break;
        
        case '+':
            tok = token::obj(token::PLUS, ch);
            break;

        case '-':
            tok = token::obj(token::MINUS, ch);
            break;
        
        case '*':
            tok = token::obj(token::STAR, ch);
            break;
        
        case '/':
            tok = token::obj(token::SLASH, ch);
            break;
        
        case '<':
            tok = token::obj(token::LT, ch);
            break;
        
        case '>':
            tok = token::obj(token::GT, ch);
            break;

        case '{':
            tok = token::obj(token::LBRACE, ch);
            break;

        case '}':
            tok = token::obj(token::RBRACE, ch);
            break;
        
        case 0:
            tok = token::obj(token::TEOF, "");
            break;

        default:
            if (is_iden(ch)) {
                string lit = read_iden();
                tok = token::obj(token::lookup_iden(lit), lit);

                return tok;
            } else if (isdigit(ch)) {
                tok = token::obj(token::INT, read_num());

                return tok;
            } else {
                tok = token::obj(token::ILLEGAL, ch);
            }
    }
        
    read_char();

    return tok;       
}
