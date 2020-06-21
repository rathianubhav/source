#pragma once
#include "st.h"


namespace source {
namespace context {

class Context {
public:
    source::ST st;
    int func_count = 0;
    int loop_count = 0;

    Context() : st(nullptr) {}

    Context(source::ST &s) : st(st) { }
};
}
}