#include <stdio.h>

extern "C"
long print(long d) {
    fprintf(stdout, "%d\n",d);
    return 0;
}