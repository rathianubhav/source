#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {
    namespace ast {
        class stmt;
    }

    namespace st {
        class obj;
    }

    namespace value {
        class fundef {
            public:
                virtual string get_args() = 0;
                virtual ast::stmt* get_body() = 0;
        };

        struct clouser {
            fundef* lambda;
            st::obj* env;
        };

        typedef enum {
            NONE,
            INT,
            STRING,
            FLOAT,
            BOOL,
            FUNC,
        } type ;

        class obj {
            private:
                int __integer__;
                string __string__;
                float __float__;
                bool __bool__;
                clouser __clouser__;
                type __type__;

            public:
                obj() : __type__(NONE) {}
                obj(int i) : __type__(INT), __integer__(i) {}
                obj(bool b) : __type__(BOOL), __bool__(b) {}
                obj(string s) : __type__(STRING), __string__(s) {}
                obj(fundef* l, st::obj* e)
                    : __type__(FUNC)
                {
                    __clouser__.lambda = l;
                    __clouser__.env = e;
                }

                int get_int() { return __integer__; }
                bool get_bool() { return __bool__; }
                string get_str() { return __string__;}
                value::type get_type() { return __type__; }

                clouser get_func() { return __clouser__;}


                obj operator+(obj& r) {
                    if (get_type() != r.get_type())
                        throw runtime_error("invalid operation between value of different type");

                    switch (get_type()) {
                        case value::INT:
                            return value::obj(get_int() + r.get_int());
                        
                        case value::STRING:
                            return value::obj(get_str() + r.get_str());
                    }

                    throw runtime_error("illegal operation perfromed");
                    return value::obj();
                }
        };
    }

}