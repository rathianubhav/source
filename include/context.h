#pragma once
#include "st.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace source {
namespace context {

class Context {
public:
    source::ST st;
    llvm::LLVMContext lc;
    llvm::IRBuilder<> lb;
    std::unique_ptr<llvm::Module> lm;
    int func_count = 0;
    int loop_count = 0;

    llvm::BasicBlock* currentBlock = nullptr;

    std::map<std::string, llvm::AllocaInst*> Table;

    Context() : st(nullptr), lb(lc) {}

    Context(source::ST &s) : st(st), lb(lc) { }
};
}
}