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
                virtual int exec(context::obj&) = 0;
                virtual string label() { return "stmt()"; }
        };

        class expr : public node {
            public:
                virtual string label() { return "expr()"; }
                virtual value::obj eval(context::obj&) = 0;
                virtual value::obj get() =  0;
        };

        class type : public node {
            protected:
                token::type type;
                std::string lit;
            public:
                virtual string label() { return "type(" + lit + ")";}

        };

        class identifier : public expr {
            private:
                std::string value;
            
            public:
                identifier(const std::string& v)
                    : value(v) {}
                virtual string label() {return "identifier(" + value + ")";}
                virtual value::obj eval(context::obj& cc) {return cc.st.lookup(value);}
                virtual value::obj get() {return value::obj(value);}
        };


        class var_decl : public stmt {
            protected:
                value::type type;
                int ptr_count = 0;
                std::unique_ptr<ast::identifier> iden;
                std::unique_ptr<ast::expr> expr = nullptr;
            
            public:
                var_decl(value::type t, unique_ptr<ast::identifier> i, int p = 0, unique_ptr<ast::expr> e = nullptr)
                    :   type(t), iden(move(i)), ptr_count(p)
                {
                    if (e)  expr = move(e);
                }
                virtual string label() { return "var_decl(" + iden->get().str_val() + ")";}
                virtual int exec(context::obj&);

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

        class null_stmt : public stmt {
            public:
                null_stmt() {}

                virtual int exec(context::obj& cc)
                {
                    return 0;
                }
        };

        class debug_stmt : public stmt {
            private:
                int dc;
            public:
                debug_stmt(int d) : dc(d) { }
                virtual int exec(context::obj& cc);
        };

        class expr_stmt : public stmt {
            private:
                std::unique_ptr<ast::expr> __expr;
            public:
                expr_stmt(unique_ptr<ast::expr> e)
                : __expr(move(e)) {}

                virtual int exec(context::obj& cc)
                {
                    io::println(__expr->eval(cc));
                    // return statement missing
                    return 0;
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
