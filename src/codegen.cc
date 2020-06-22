#include <node.h>

llvm::Type*
IntType::gen(context::Context &cc){
    switch(size) {
        case 0: return llvm::Type::getInt64Ty(cc.lc);
        case 1: return llvm::Type::getInt1Ty(cc.lc);
        case 8: return llvm::Type::getInt8Ty(cc.lc);
        case 16: return llvm::Type::getInt16Ty(cc.lc);
        case 32: return llvm::Type::getInt32Ty(cc.lc);
        case 64: return llvm::Type::getInt64Ty(cc.lc);
    }

    err << "error: illegal size of 'int' " << size << std::endl;
    exit(EXIT_FAILURE);
    return nullptr;
}

llvm::Value*
Identifier::codegen(context::Context& cc)
{
    return cc.st.lookup(value).get();
}

llvm::Value*
Number::codegen(context::Context& cc)
{
    IntType i64_type(0,true);
    return llvm::ConstantInt::get(i64_type.gen(cc), atoi(value.c_str()), false);
}


llvm::Value*
Arithmetic::codegen(context::Context& cc)
{
    llvm::Value* l = left->codegen(cc);
    llvm::Value* r = right->codegen(cc);
    if (!l || !r) return nullptr;

    switch(op) {
        case ADD: return cc.lb.CreateAdd(l, r, "addtmp");
        case SUB: return cc.lb.CreateSub(l, r, "subtmp");
        case MUL: return cc.lb.CreateMul(l, r, "multmp");
        case DIV: return cc.lb.CreateUDiv(l, r, "divtmp");
        default:
            err << "illegal operator" << std::endl;
            return nullptr;
    }
}


llvm::Value*
Block::codegen(context::Context &cc)
{
    llvm::Value* retval;
    for(auto a : *body) retval = a->codegen(cc);
    return retval;
}

llvm::Function*
Prototype::codegen(context::Context& cc)
{
    std::vector<llvm::Type*> llargs;

    for(auto arg : *args) {
        
        llargs.push_back(arg->t->gen(cc));
    }

    llvm::FunctionType* ft;

    ft = llvm::FunctionType::get(rettype->gen(cc), llargs, false);

    llvm::Function* f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, id->get(), cc.lm.get());

    return f;
}

llvm::Function*
Method::codegen(context::Context &cc)
{
    llvm::Function *f = cc.lm->getFunction(proto->getid());
    if (!f) f = proto->codegen(cc);
    if (!f) return nullptr;

    if (!f->empty()) {
        err << "Error: '" << proto->getid() << "' is already defined"<< std::endl;
        return nullptr;
    }

    llvm::BasicBlock* bb = llvm::BasicBlock::Create(cc.lc, "entry", f);
    cc.lb.SetInsertPoint(bb);

    body->codegen(cc);    
    llvm::verifyFunction(*f);
    return f;
}

llvm::Value*
Call::codegen(context::Context &cc)
{
    llvm::Function *callef = cc.lm->getFunction(id->get());
    if (callef) {
        err << "Error: '" << id->get() << "' function is called before defination" << std::endl;
        return nullptr;
    }

    if (callef->arg_size() != arg->size()) {
        err << "Error: '" << id->get() << "' function need to have " << callef->arg_size() << ", but " << arg->size() << " (is)are provided" << std::endl;
        return nullptr;
    }

    std::vector<llvm::Value*> argv;
    for(int i = 0, e = arg->size(); i != e; ++i) {
        argv.push_back(arg->at(i)->codegen(cc));
        if (!argv.back()) return nullptr;
    }

    return cc.lb.CreateCall(callef, argv, "calltmp");
}


llvm::Value*
Return::codegen(context::Context& cc)
{
    return cc.lb.CreateRet(expr->codegen(cc));
}