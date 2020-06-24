#include <node.h>

llvm::AllocaInst*
allocateInst(llvm::BasicBlock* bb, const std::string &n, llvm::Type *t)
{
    llvm::IRBuilder<> tmpb(bb, bb->begin());
    return tmpb.CreateAlloca(t, 0, n.c_str());
}


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

llvm::Type*
FloatType::gen(context::Context &cc){
    switch(size) {
        case 0: return llvm::Type::getFloatTy(cc.lc);
        case 32: return llvm::Type::getFloatTy(cc.lc);
        case 64: return llvm::Type::getDoubleTy(cc.lc);
    }

    err << "error: illegal size of 'float' " << size << std::endl;
    exit(EXIT_FAILURE);
    return nullptr;
}

llvm::Value*
Identifier::codegen(context::Context& cc)
{
    return cc.Table[value];
}

llvm::Value*
Number::codegen(context::Context& cc)
{
    for(auto a : value) {
        if (a == '.') {
            FloatType ftype;
            return llvm::ConstantFP::get(ftype.gen(cc), atof(value.c_str()));
        }
    }
    IntType i64_type(0,true);
    return llvm::ConstantInt::get(i64_type.gen(cc), (uint64_t) atoi(value.c_str()), false);
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

    uint idx = 0;
    for (auto &arg : f->args()) {
        arg.setName(args->at(idx++)->name->get());
    }
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

    cc.currentBlock = bb;
    cc.Table.clear();
    for(auto &arg: f->args()) {
        llvm::AllocaInst *alloc = allocateInst(bb, arg.getName(), arg.getType());
        cc.lb.CreateStore(&arg, alloc);
        cc.Table[arg.getName()] = alloc;
    }
    body->codegen(cc);    
    llvm::verifyFunction(*f);
    return f;
}

llvm::Value*
Call::codegen(context::Context &cc)
{
    llvm::Function *callef = cc.lm->getFunction(id->get());
    if (!callef) {
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


llvm::Value*
Let::codegen(context::Context& cc)
{
    auto dt = t->gen(cc);
    if (!dt) {
        err << "variable '" << id->get() << "' is declared with unknown datatype '" << typeid(t).name() << std::endl;
        exit(1);
    }

    llvm::AllocaInst *alloc = allocateInst(cc.currentBlock, id->get(), dt);

    if (expr) {
        auto exp = expr->codegen(cc);
        cc.lb.CreateStore(exp, alloc);
    }

    cc.Table[id->get()] = alloc;

    return nullptr;
}


llvm::Value*
Condition::codegen(context::Context &cc)
{
    auto conv = clause->codegen(cc);
    if (!conv) {
        err << "error while generating code for condition" << std::endl;
        return nullptr;
    }

    IntType i64_type;
    conv = cc.lb.CreateFCmpONE(
        conv, llvm::ConstantInt::get(i64_type.gen(cc), (uint64_t) 0, false), "ifcond"
    );

    auto curb = cc.currentBlock;

    llvm::Function* f = cc.lb.GetInsertBlock()->getParent();

    llvm::BasicBlock* ifb = llvm::BasicBlock::Create(cc.lc, "then", f),
                    * elb = llvm::BasicBlock::Create(cc.lc, "else"),
                    * mb  = llvm::BasicBlock::Create(cc.lc, "ifcont");
    cc.lb.CreateCondBr(conv, ifb, elb);

    cc.lb.SetInsertPoint(ifb);
    cc.currentBlock = ifb;

    llvm::Value *ifbv = ifblock->codegen(cc);
    if (!ifbv) return nullptr;

    cc.lb.CreateBr(mb);

    ifb = cc.lb.GetInsertBlock();

    llvm::Value *elbv;
    if (elblock) {

        f->getBasicBlockList().push_back(elb);
        cc.lb.SetInsertPoint(elb);
        cc.currentBlock = elb;

        elbv = elblock->codegen(cc);
        if (!elbv) {
            return nullptr;
        }

        cc.lb.CreateBr(mb);

        elb = cc.lb.GetInsertBlock();
    }

    f->getBasicBlockList().push_back(mb);
    cc.lb.SetInsertPoint(mb);
    llvm::PHINode *pn = cc.lb.CreatePHI(llvm::Type::getInt64Ty(cc.lc), 2, "iftmp");

    pn->addIncoming(ifbv, ifb);
    if (elblock) {
        pn->addIncoming(elbv, elb);
    }

    return pn;

}

llvm::Value*
ExpressionStatment::codegen(context::Context& cc)
{
    return expr->codegen(cc);
}