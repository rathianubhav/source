#pragma once
#include "st.h"


namespace source {
namespace context {

class Context {
public:
    int loop = 0;
    int func = 0;
    source::ST *st;
};

Context* init(ST* st = nullptr);

}
}