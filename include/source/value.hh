#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {

    namespace value {
        typedef enum {
            NONE,
            INT,
            STRING,
            FLOAT,
            BOOL
        } type ;

        class obj {
            private:
                int __integer__;
                string __string__;
                float __float__;
                bool __bool__;
                type __type__;

            public:
                obj() : __type__(NONE) {}
                obj(int i) : __type__(INT), __integer__(i) {}
                obj(bool b) : __type__(BOOL), __bool__(b) {}
                obj(string s) : __type__(STRING), __string__(s) {}

                int get_int() { return __integer__; }
                bool get_bool() { return __bool__; }
                string get_str() { return __string__;}
                value::type get_type() { return __type__; }
        };
    }

}