#pragma once

#include "ast.hh"


namespace source {
    namespace parser {
        class obj {
            private:
                unique_ptr<lexer::obj> __lexer__;
                context::obj __context__;
                token::obj curtok;
                token::obj peektok;

                void eat_token();

                void expect(token::type t);

                unique_ptr<ast::stmt> parse_stmt();
                unique_ptr<ast::stmt> parse_let();
                unique_ptr<ast::stmt> parse_expr_stmt();


                unique_ptr<ast::expr> parse_expr();
                unique_ptr<ast::expr> parse_ident();
                unique_ptr<ast::expr> parse_num();

            public:
                obj(unique_ptr<lexer::obj> l, context::obj& cc)
                    : __lexer__(move(l)), __context__(cc)
                {
                    eat_token();
                    eat_token();
                }

                unique_ptr<ast::program> parse();
        };
    }
}
