#include <releax-cc/codegen>
#include <compiler.h>
extern bool error;
extern bool isdebug;

int yyparse();
std::vector<Statment*> *tree;

void
source: