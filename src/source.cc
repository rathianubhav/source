#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>

#ifdef __releax__
#include <releax>
#endif

using namespace std;

#include "../include/ast"
#include "../include/inbuilts"

extern FILE* yyin;
extern Stmt* tree;
extern int yyparse();
extern int yylex_destroy();


void
setup_global(Frame & g) 
{
    vector<InBuilt*> inbuilts = {
        new ShowVar(), new Dlopen(), new Dlsym(), new Dlexec(0),new Dlexec(1), new Dlexec(2), new Dlexec(3), new Dlclose(), 
        new IsType("int",INT_T), new IsType("float", FLOAT_T), new IsType("str", STR_T), new IsType("bool", BOOL_T), new IsType("func",FUNC_T), new Sleep(),
        new System(), new CStr()
    };
    for (auto a : inbuilts) {
        g.bind(a->get_name(), Value(a,nullptr));
    }

    g.bind("null",Value());

    #ifdef __releax__
        g.bind("os",Value("releax"));
    #elif __unix__
        g.bind("os",Value("unix"));
    #elif __FreeBSD__
        g.bind("os", Value("freebsd"));
    #elif _WIN64
        g.bind("os",Value("windows"));
    #else
        g.bind("os",Value("unknown"));
    #endif

}

int 
main(int ac, char** av)
{
    showPrompt = isatty(0) && isatty(2);
    bool interactive = isatty(1) && isatty(2);

    if (ac >= 2) {
        if (!(yyin = fopen(av[1], "r"))) {
            cerr << "unable to open source file '" << av[1] << "'" << endl;
            exit(2);
        }

        interactive = false;
    }

    Frame global(nullptr);
    vector<Stmt*> program;


    setup_global(global);
   
    int i = 0;
    
    while(true) {
        tree = nullptr;
        error = false;
        if (interactive) cerr << "source > " ;
        if (yyparse() != 0) error = true;
        if (!error) {
            if (tree) {
                tree->exec(&global);
                program.push_back(tree);
            }
            else break;
        }
        //cout << "in external loop" << endl;
    }

    auto mainID = new Id("main");
    global.bind("arg", Value("hello"));
    auto args = new vector<Expr*>{ new Id("arg")};

    auto fc = new Call(mainID, args);
    fc->eval(&global);

    if (interactive) cerr << "exit()" << endl;

    if (ac >= 2) fclose(yyin);
    for (Stmt* node : program) {
        if (node != nullptr) delete node;
    }

    delete args;
    delete fc;

    yylex_destroy();

    if (error) return 5;
    else return 0;
}