#pragma once

#include "lexer.hh"
#include "context.hh"
#include "io.hh"

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
                virtual value::obj get() =  0;
        };

        class constant : public expr {
            private:
                value::obj v;
            public:
                constant(value::obj v) : v(v) {}
                virtual string label() { return "constant()"; }
                virtual value::obj eval(context::obj&) {return v;}
                virtual value::obj get() {return v;}
        };

        class expr_stmt : public stmt {
            private:
                std::unique_ptr<ast::expr> __expr;
            public:
                expr_stmt(unique_ptr<ast::expr> e)
                : __expr(move(e)) {}

                virtual void exec(context::obj& cc)
                {
                    io::println(__expr->eval(cc));
                }
        };

        class num : public expr {
            private:
                int val;
            public:
                num(int v) {val = v;}
                virtual string label() { return "num(" + to_string(val) +")"; }
                virtual value::obj get() { return value::obj(val); }
                virtual value::obj eval(context::obj&) { return value::obj(val); }
        };

        class program {
            public:
                vector<unique_ptr<stmt>> stmts;

                program() { }

                string label();
        };

    }
}
