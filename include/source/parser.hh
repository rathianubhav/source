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

                unique_ptr<ast::stmt> parse_stmt();
                unique_ptr<ast::stmt> parse_let();
                unique_ptr<ast::stmt> parse_print_stmt();
                unique_ptr<ast::stmt> parse_if_stmt();
                unique_ptr<ast::stmt> parse_while_stmt();
                unique_ptr<ast::stmt> parse_expr_stmt();
                unique_ptr<ast::stmt> parse_assign_stmt();


                unique_ptr<ast::expr> parse_expr();
                unique_ptr<ast::expr> parse_op_expr();
                unique_ptr<ast::expr> parse_ident();
                unique_ptr<ast::expr> parse_num();
                unique_ptr<ast::expr> parse_str();

                unique_ptr<ast::expr> parse_func();
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
