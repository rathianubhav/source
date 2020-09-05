#include <source/lexer.hh>

using namespace source;

bool
token::is_operator(token::type t)
{
    return t == token::plus  ||
           t == token::minus ||
           t == token::star  ||
           t == token::slash;
}

map<string, token::type> __keywords__ {
    {"func", token::__func},
    {"let", token::__let},
    {"if", token::__if},
    {"else", token::__else},
    {"for", token::__for},
    {"print", token::__print},
    {"while", token::__while},
};

token::type
token::lookup_iden(const string &iden)
{
    if (__keywords__.find(iden) != __keywords__.end())
        return __keywords__[iden];
    
    return token::ident;
}

string
token::tok_to_str(token::type t)
{
    switch (t) {
        case token::illegal: return "illegal";
        case token::eof: return "teof";
        case token::ident: return "ident";
        case token::__int: return "int";
        case token::assign: return "assign";
        case token::plus: return "plus";
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

    col = pos;
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
        read_char();
    }
        
        
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
lexer::obj::read_str()
{
    int p = pos;
    read_char();
    while(ch != '"') read_char();
    return input.substr(p + 1, pos - (p + 1));
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
                tok = token::obj(token::eq, "==");
            } else {
                tok = token::obj(token::assign, ch);
            }
            break;

        case '!':
            if (peek_char() == '=') {
                read_char();
                tok = token::obj(token::ne, "!=");
            } else {
                tok = token::obj(token::bang, ch);
            }
        
        case ';':
            tok = token::obj(token::semicolon, ch);
            break;
        
        case '(':
            tok = token::obj(token::lparen, ch);
            break;

        case ')':
            tok = token::obj(token::rparen, ch);
            break;

        case ',':
            tok = token::obj(token::comma, ch);
            break;
        
        case '+':
            tok = token::obj(token::plus, ch);
            break;

        case '-':
            tok = token::obj(token::minus, ch);
            break;
        
        case '*':
            tok = token::obj(token::star, ch);
            break;
        
        case '/':
            tok = token::obj(token::slash, ch);
            break;
        
        case '<':
            tok = token::obj(token::lt, ch);
            break;
        
        case '>':
            tok = token::obj(token::gt, ch);
            break;

        case '{':
            tok = token::obj(token::lbrace, ch);
            break;

        case '}':
            tok = token::obj(token::rbrace, ch);
            break;

        case '"':
            tok = token::obj(token::__string, read_str());
            break;
        
        case 0:
            tok = token::obj(token::eof, "");
            break;

        default:
            if (is_iden(ch)) {
                string lit = read_iden();
                token::type type = token::lookup_iden(lit);
                return token::obj(type, lit);

            } else if (isdigit(ch)) {
                string num =  read_num();
                return token::obj(token::__int, num);
            } else {
                tok = token::obj(token::illegal, ch);
            }
    }

  
    read_char();

    return tok;       
}


void
lexer::obj::throw_error(const string& mesg)
{
    cout << "Error: [" << line << ":" << col << "] - " << mesg << endl;
}