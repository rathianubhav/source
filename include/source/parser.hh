#pragma once

#include "ast.hh"


namespace source {
    namespace parser {
        class obj {
            private:
                lexer::obj __lexer__;
                context::obj __context__;
                token::obj curtok;
                token::obj peektok;

                void eat_token();

                void expect(token::type t);

                unique_ptr<ast::stmt> parse_root_declaration();

                unique_ptr<ast::stmt> parse_debug_stmt();
                
                unique_ptr<ast::expr> parse_expr();
                unique_ptr<ast::expr> parse_expr_tail(unique_ptr<ast::expr>);

                unique_ptr<ast::expr> parse_term();
                unique_ptr<ast::expr> parse_term_tail(unique_ptr<ast::expr>);

                unique_ptr<ast::expr> parse_factor();

                unique_ptr<ast::stmt> parse_expr_stmt();

            public:
                obj(lexer::obj& l, context::obj& cc)
                    : __lexer__(l), __context__(cc)
                {
                    eat_token();
                    eat_token();
                }

                unique_ptr<ast::program> parse();
        };
    }
}
