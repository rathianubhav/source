#pragma once

#include "value.hh"

namespace source {
    namespace st {
        class obj {
            private:
                map<string, value::obj> __table__;

            public:
                obj() {}

                void insert(const string& s, value::obj val);
                void update(const string& s, value::obj val);
                value::obj lookup(const string &s);
        };
    }
}
