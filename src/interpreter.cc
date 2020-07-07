#include <symboltable.h>
#include <node.h>
#include <source.h>
#include <inbuilts.h>


extern FILE *yyin;
int yyparse();
vector<Statment*> *tree;
extern vector<InBuilt*> inbuilts;

bool interactive = false;

void
interprete(const char* filename)
{
    if (filename != nullptr) {
        yyin = fopen(filename, "r");
        interactive = false;
    } else {
        interactive = true;
    }

    Context cc;

    for(auto a : inbuilts) {
        cc.st.bind(a->get_id(), Value(a, nullptr));
    }

    vector<Statment*> program;

    if (interactive) {
        cout << "Source interactive mode" << endl;
        while (true) {
            cout << "source> ";
            tree = nullptr;
            yyparse();

            for(auto a : *tree) {
                a->exec(cc);
                program.push_back(a);
            }
        }

        for(auto a : program) {
            delete a;
        }
        program.clear();
        
    } else {
        yyparse();
        for(auto s : *tree) {
            s->exec(cc);
        }
    }
    
    for(auto a : inbuilts) {
        delete a;
    }
    inbuilts.clear();
}


