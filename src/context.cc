#include <context.h>

source::context::Context*
source::context::init(ST* st)
{
    auto cc = new source::context::Context();
    cc->st = new source::ST(st);

    return cc;
}