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
