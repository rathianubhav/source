#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {
    namespace token {
        typedef enum {
            illegal,
            eof,
            ident,
            assign,

            num,
            decimal,
            str,

            plus,
            minus,
            bang,
            star,
            slash,

            lt,
            gt,
            eq,
            ne,

            comma,
            semicolon,
            colon,

            lparen,
            rparen,
            lbrace,
            rbrace,

            __func,
            __let,
            __true,
            __false,
            __if,
            __else,
            __for,
            __while,
            __ret,
            __print,
            __do,
            __auto,
            __break,
            __case,
            __char,
            __int,
            __float,
            __string,

            _const,

            __continue,
            __default,
            __extern,
            __goto,
            __register,
            __return,
            __sizeof,
            __static,
            __switch,

        } type;

        bool is_operator(token::type);

        class obj {
            private:
                token::type __type__;
                string __lit__;
            public:
                obj() {}

                obj(token::type t, const string& s)
                    : __type__(t), __lit__(s) {}

                obj(token::type t, int l)
                    : __type__(t), __lit__(string(1,l)) {}
                
                obj(token::type t, char c)
                    : __type__(t), __lit__(string(1, c)) {}

                string get_lit() { return __lit__; }

                token::type get_type() { return __type__;}
                

        };

        string tok_to_str(token::type t);

        token::type lookup_iden(const string &iden);
    }

    namespace lexer {
        class obj {
            private:
                string input;
                int pos, read_pos;
                int ch;
            
            public:
                int line, col;

                obj(const string& i)
                    : input(i)
                {
                    read_pos = 0;
                    line = 0; col = 0;
                    read_char();
                }
                
                void read_char();
                string read_iden();
                string read_num();
                string read_str();

                bool is_iden(char c);
                void skip_spaces();
                char peek_char();

                token::obj next_token();

                void throw_error(const string& mesg);

                static void __test__(const string& , vector<token::obj>);
                
        };
    }
}

void test_lexer();
