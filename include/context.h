#pragma once
#include "st.h"


namespace source {
namespace context {

class Context {
public:
    source::ST *st;
    int func_count = 0;
    int loop_count = 0;
};

Context* init(ST* st = nullptr);

}
}