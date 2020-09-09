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
                value::obj eval(context::obj&);
        };

        class num : public expr {
            private:
                int val;

            public:
                num(int val) : val(val) {}
                string label() { return "num(" + to_string(val) + ")";}
                value::obj eval(context::obj&) { return value::obj(val);}
        };

        class str : public expr {
            private:
                string val;
            
            public:
                str(const string& s) : val(s) {}
                string label() {return "str(" + val + ")";}
                value::obj eval(context::obj&) { return value::obj(val); }
        };

        class binexpr : public expr {
            private:
                unique_ptr<expr> left, right;
                token::type oper;
            
            public:
                binexpr(unique_ptr<expr> l, token::type o, unique_ptr<expr> r)
                    : left(move(l)), right(move(r)), oper(o)
                {}

                value::obj eval(context::obj& );
                string label();

        };

        class let : public stmt {
            private:
                unique_ptr<ident> id;
                unique_ptr<expr> val;
            
            public:
                let(unique_ptr<ident> id, unique_ptr<expr> val)
                    : id(move(id)), val(move(val)) {}

                void exec(context::obj& cc) { 
                    cc.st.insert(id->get(), val->eval(cc)); 
                }

                string label();

        };

        class assign : public stmt {
            private:
                unique_ptr<ident> id;
                unique_ptr<expr> val;
            
            public:
                assign(unique_ptr<ident> id, unique_ptr<expr> val)
                    : id(move(id)), val(move(val)) {}

                void exec(context::obj& cc) { cc.st.update(id->get(), val->eval(cc)); }

                string label();
        };

        class method : public expr, public value::fundef {
            private:
                unique_ptr<ident> var;
                stmt* body;
            
            public:
                method(stmt* s)
                    : body(s) {}
                
                string get_args() {return var->get(); }
                stmt* get_body() {
                    if (body == nullptr) {
                        cout << "body is null" << endl;
                    }
                    return body;
                }

                value::obj eval(context::obj& cc) {
                    return value::obj(this, &cc.st);
                }

        };

        class call : public expr {
            private:
                unique_ptr<expr> id, arg;
            
            public:
                call(unique_ptr<ident> id)
                    : id(move(id)) {}
                
                value::obj eval(context::obj &cc);
        };




        class print_stmt : public stmt {
            private:
                unique_ptr<expr> e;
            
            public:
                print_stmt(unique_ptr<expr> e)
                    : e(move(e)) {}
                
                void exec(context::obj& cc);
        };


        class condition : public stmt {
            private:
                unique_ptr<expr> e;
                unique_ptr<stmt> s;

            public:
                condition(unique_ptr<expr> e, unique_ptr<stmt> s)
                    : e(move(e)), s(move(s))
                {}

                void exec(context::obj& cc);
        };

        class nullstmt : public stmt {
            private:
            
            public:
                nullstmt() {}

                void exec(context::obj& cc) {}
        };

        class while_loop : public stmt {
            private:
                unique_ptr<expr>  __expr;
                unique_ptr<stmt> __stmt;

            public:
                while_loop(unique_ptr<expr> e, unique_ptr<stmt> s)
                    : __expr(move(e)), __stmt(move(s))
                {}

                void exec(context::obj& cc) {
                    
                    while (__expr->eval(cc).get_bool()) {
                        __stmt->exec(cc);
                    }

                }
        };

        class do_loop : public stmt {
            private:
                unique_ptr<expr> __expr;
                unique_ptr<stmt> __stmt;
            
            public:
                do_loop(unique_ptr<expr> e,unique_ptr<stmt> s)
                    : __expr(move(e)), __stmt(move(s))
                {}

                void exec(context::obj& cc) {

                    do {
                        __stmt->exec(cc);
                    } while(__expr->eval(cc).get_bool());
                }
        };
    }
}
