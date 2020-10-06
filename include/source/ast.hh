#pragma once

#include "lexer.hh"
#include "context.hh"


namespace source {
    namespace ast {
        class node {
            public:
                virtual ~node() {}
                virtual string label() { return "node()"; }
        };

        class stmt : public node {
            public:
                virtual void exec(context::obj&) = 0;
                virtual string label() { return "stmt()"; }
        };

        class expr : public node {

            public:
                virtual string label() { return "expr()"; }
                virtual value::obj eval(context::obj&) = 0;
        };

        class program {
            public:
                vector<unique_ptr<stmt>> stmts;

                program() { }

                string label();
        };

    }
}
