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

        class ident : public expr {
            private:
                string val;
            public:
                ident(const string& val) : val(val) {}

                string label() { return "ident(" + val + ")";}
                string get() { return val;}
                value::obj eval(context::obj&) { return value::obj(val); }
        };

        class num : public expr {
            private:
                int val;

            public:
                num(int val) : val(val) {}
                string label() { return "num(" + to_string(val) + ")";}
                value::obj eval(context::obj&) { return value::obj(val);}
        };

        class let : public stmt {
            private:
                unique_ptr<ident> id;
                unique_ptr<expr> val;
            
            public:
                let(unique_ptr<ident> id, unique_ptr<expr> val)
                    : id(move(id)), val(move(val)) {}

                void exec(context::obj& cc) { cc.st.insert(id->get(), val->eval(cc)); }

                string label();

        };

        class call : public expr {
            private:
                unique_ptr<ident> id;
            
            public:
                call(unique_ptr<ident> id)
                    : id(move(id)) {}
                
                value::obj eval(context::obj &cc) { return value::obj(); }
        };
    }
}
