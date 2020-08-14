#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {
    namespace token {
        typedef enum {
            ILLEGAL,
            TEOF,
            IDENT,
            INT,
            ASSIGN,
            PLUS,
            MINUS,
            BANG,
            STAR,
            SLASH,

            LT,
            GT,
            EQ,
            NE,

            COMMA,
            SEMICOLON,

            LPAREN,
            RPAREN,
            LBRACE,
            RBRACE,

            FUNC,
            LET,
            TRUE,
            FALSE,
            IF,
            ELSE,
            FOR,
            RETURN
        } type;

        class obj {
            private:
                token::type __type__;
                string __lit__;
            public:
                obj() {}

                obj(token::type t, const string& s)
                    : __type__(t), __lit__(s) {}
                
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

                bool is_iden(char c);
                void skip_spaces();
                char peek_char();

                token::obj next_token();
                
        };
    }
}
