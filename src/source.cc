#include <source.h>

int
main(int ac, char** av)
{
    if (ac >= 2) interprete(av[1]);
    else interprete(nullptr);

    return 0;
}