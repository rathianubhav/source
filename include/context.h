#pragma once
#include "st.h"


namespace source {
namespace context {

class Context {
public:
    source::ST *st;
};

Context* init(ST* st = nullptr);

}
}