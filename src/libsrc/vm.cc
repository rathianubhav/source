#include <source/vm.hh>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace source;

void
vm::obj::insert(int v)
{
    __text_segment[i++] = v;
}


void
vm::obj::initialize_pool()
{
    int pool_size = 1000;
    __text_segment = __old_text_segment = (int*) malloc(pool_size);
    __data_segment = (char*) malloc(pool_size);
    __stack  = (int*) malloc(pool_size);

    memset(__text_segment, 0, pool_size);
    memset(__data_segment, 0, pool_size);
    memset(__stack, 0, pool_size);

    bp = sp = (int*)((int) __stack + pool_size);
    ax = 0;
}

void
vm::obj::eval()
{
    pc == __text_segment;

    int op, *tmp;
    while(1)
    {
        op = *pc++;
        switch(op)
        {
            // Load Immediate value to ax;
            case IMM:   ax = *pc++;         break;

            // load character to ax;
            case LC:    ax = *(char*)ax;    break;

            // load int to ax;
            case LI:    ax = *(int*)ax;     break;

            // save char to addr, value in ax, addr on stack
            case SC:    ax = *(char*) *sp++ = ax;   break;
            
            // save int to addr, value in ax, addr on stack
            case SI:   *(int*) *sp++ = ax;  break;

            // push the value of ax onto stack
            case PUSH:  *--sp = ax;

            // jump to addr
            case JMP:   pc = (int*)*pc;

            // jump to addr if ax is zero
            case JZ:    pc = ax ? pc + 1 : (int*)*pc;

            // jump to addr if ax is not zero
            case JNZ:   pc = !ax ? pc + 1 : (int*)*pc;

            // call subroutine
            case CALL:  *--sp = (int)(pc+1);    pc = (int*)*pc;

            // return to subroutine
            case RET:   pc = (int*)*sp++;
        }
    }
}