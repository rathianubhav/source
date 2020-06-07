#ifndef __COMPILER__
#define __COMPILER__

#include "buffer.hh"
using namespace std;

class Stmt;

extern Buffer codes;
extern Buffer errout;
extern Stmt* tree;

class Context {
    private:
        int reg_count = 0;

    public:
        string next_register() {
            return "%v" + to_string(++reg_count);
        }
};

#endif