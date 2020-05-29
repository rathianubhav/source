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
Frame global(nullptr);
vector<Stmt*> program;

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

    bool show_graph = false;
    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "--graph")) {
            show_graph = true;
        }
    }

    vector<InBuilt*> inbuilts = {
        new ShowVar(), new Dlopen(), new Dlsym(), new Dlexec(0),new Dlexec(1), new Dlexec(2), new Dlexec(3), new Dlclose(), 
        new IsType("int",INT_T), new IsType("float", FLOAT_T), new IsType("str", STR_T), new IsType("bool", BOOL_T), new IsType("func",FUNC_T), new Sleep()
    };
    for (auto a : inbuilts) {
        global.bind(a->get_name(), Value(a,nullptr));
    }

    global.bind("null",Value());

    #ifdef __releax__
        global.bind("os",Value("releax"));
    #elif __unix__
        global.bind("os",Value("unix"));
    #elif __FreeBSD__
        global.bind("os", Value("freebsd"));
    #elif _WIN64
        global.bind("os",Value("windows"));
    #else
        global.bind("os",Value("unknown"));
    #endif

    int i = 0;
    
    while(true) {
        tree = nullptr;
        error = false;
        if (interactive) cerr << "source > " ;
        if (yyparse() != 0) error = true;
        if (!error) {
            if (tree) {
                if (show_graph) {
                    tree->write_dot("build/graph/source_" + to_string(++i) + ".dot");
                    string cmd = "dot -Tpng build/graph/source_" + to_string(i) + ".dot -o build/graph/source_" + to_string(i) + ".png";
                    if (system(cmd.c_str()) != 0)
                    {
                        cout << "error while displaying tree" << endl;
                    }
                //cout << "executing Node " << tree->node_label << endl;
                }
                tree->exec(&global);
                program.push_back(tree);
            }
            else break;
        }
        //cout << "in external loop" << endl;
    }

    auto mainID = new Id("main");
    global.bind("arg", Value("hello"));
    auto args = new vector<Exp*>{ new Id("arg")};

    auto fc = new FunCall(mainID, args);
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