#include <fstream>
#include <vector>
#include <node.h>
#include <releax>

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

    cc.lm->print(llvm::errs(), nullptr);

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
