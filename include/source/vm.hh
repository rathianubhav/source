#ifndef __VM__
#define __VM__

namespace source::vm {
    enum INSTRUCTIONS_t {
        LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, RET,
        SI, SC, PUSH, OR, XOR, AND, EQ, NE, LT, GT, LE, SHL, SHR,
        ADD, SUB, DIV, MUL, MOD, OPEN, READ, CLOS, PRTF, MALC, 
        MSET, MCMP, EXIT
    };

    class obj {
        private:
            int *__text_segment,
                *__old_text_segment,
                *__stack;
            
            char *__data_segment;
            
            // Registers
            int *pc, *bp, *sp, ax, cycle;

            int i = 0;
        
        public:
            void initialize_pool();

            void eval();

            void insert(int v);
    };
}

#endif