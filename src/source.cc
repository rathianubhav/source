#include <fstream>
#include <vector>
#include <node.h>
#include <releax>

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

source::out::stream err(2, 31);
source::out::stream debug(1, 34);

extern "C" FILE *yyin;
std::vector<TopLevel*> *tree;
int yyparse();

bool isdebug = false;

int
source_main(releax::cli& app)
{

    isdebug = app.is_flagset("debug");
   if (app.args.size() >= 1) {
       std::string filename = app.args.at(0);
       if (releax::is_exist(filename)) {
           yyin = fopen(filename.c_str(), "r");
       }
    }
    yyparse();

    source::context::Context cc;

    cc.lm = std::make_unique<llvm::Module>("my cool jit", cc.lc);
    for(auto a: *tree) {
        a->codegen(cc);
    }

    if (app.is_flagset("ll")) {
        cc.lm->print(llvm::errs(), nullptr);
    }

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    cc.lm->setTargetTriple(targetTriple);

    std::string Error;
    auto t = llvm::TargetRegistry::lookupTarget(targetTriple, Error);

    if (!t) {
        llvm::errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto ttm = t->createTargetMachine(targetTriple, CPU, Features, opt, rm);

    cc.lm->setDataLayout(ttm->createDataLayout());

    auto filename = "output.o";

    std::error_code ec;
    llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::OF_None);

    if (ec) {
        llvm::errs() << "Could not open file: " << ec.message();
        return 1;
    }

    llvm::legacy::PassManager pass;
    auto filetype = llvm::CGFT_ObjectFile;
    if (ttm->addPassesToEmitFile(pass, dest, nullptr, filetype)) {
        llvm::errs() << "can't emit a file of this type";
        return 1;
    }

    pass.run(*cc.lm);
    dest.flush();

    debug << "'" << filename << " writter\n";

    return 0;
}

int 
main(int ac, char** av) 
{
    releax::cli app;

    app.name("source")
       .version(0.1)
       .release('a')
       .description("programming language for releax os")
       .author("Manjeet Singh", "itsmanjeet@releax.in", "author")
       .main(source_main);

    return app.execute(ac, av);

}
