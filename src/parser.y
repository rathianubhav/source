%code requires {

#include "../include/node.h"

using namespace source;
int yylex(void);
int yylex_destroy();

extern std::vector<Statment*> *tree;

void yyerror(const char* e);

}

%define parse.error verbose

%union {
    Identifier* id;
    Expression *expr;
    String* str;
    Statment* stmt;
    Block* block;
    std::vector<Statment*> *arr;
    std::vector<Expression*> *arrE;
    std::vector<Identifier*> *arrI;
    Number* num;
    dict* adict;
    std::vector<dict*>* arrD;
    Oper op;
};

%left<op> BOP
%left<op> NOTTOK
%left<op> COMP
%left<op> OPA
%left<op> OPM
%right POSNEG

%token<id> ID
%token<expr> NUM BOOL STRING

%token FUNC ASSIGN LET FOR IF ELSE PRINT PRINTLN USE SNULL IN CONT BREAK CONTINUE


%type<expr> value expr
%type<expr> math_expr func_expr arr_expr cont_expr cont_eval
%type<stmt> stmt
%type<stmt> assign_stmt condit_stmt loop_stmt expr_stmt print_stmt use_stmt
%type<block> block
%type<adict> arg_def

%type<arr> program stmts
%type<arrE> exprs
%type<arrI> ids
%type<arrD> arg_defs

%destructor {delete $$;} <block>
%destructor {delete $$;} <stmt>
%destructor {delete $$;} <expr>

%start program

%%

program
: stmts {$$=$1; tree=$$;}
;


stmt
: assign_stmt
| condit_stmt
| loop_stmt
| expr_stmt
| print_stmt
| use_stmt
| BREAK ';' {$$=new Break();}
| CONTINUE ';' {$$=new Continue();}
;



assign_stmt
: LET ID ASSIGN expr ';' {$$=new Let($2, $4);}
| ID ASSIGN expr ';' {$$=new Assign($1, $3);}
;

condit_stmt
: IF expr block {$$=new Condition($2,$3);}
| IF expr block ELSE block {$$=new Condition($2, $3, $5);}
;


loop_stmt
: FOR expr block {$$=new Loop($2, $3);}
| FOR ID IN expr block {$$=new Loop($2, $4, $5);}
;


expr_stmt
: expr ';' {$$=new ExpressionStatment($1);}
;

print_stmt
: PRINT '(' exprs ')' ';' {$$=new Print($3);}
| PRINTLN '(' exprs ')' ';' {$$=new Println($3);}
;


use_stmt
: USE STRING ';' {$$=new Use($2);}
;

block
: '{' stmts '}' {$$=new Block($2);}
;

stmts
: stmts stmt {$$=$1; $$->push_back($2);}
| {$$=new std::vector<Statment*>();}
;

expr
: math_expr
| func_expr
| value
| '(' expr ')' {$$=$2;}
| arr_expr
| cont_expr
| SNULL {$$=new Null();}
| ID '[' expr ']' {$$=new Access($1, $3);}
| cont_eval
;


math_expr
: expr COMP expr {$$=new Compare($1, $2, $3);}
| expr OPA expr {$$=new Arithmetic($1, $2, $3);}
| expr OPM expr {$$=new Arithmetic($1, $2, $3);}
| expr BOP expr {$$=new Logical($1, $2, $3);}
| NOTTOK expr {$$=new Negation($2);}
;

func_expr
: FUNC ids block {$$=new Method($2, $3);}
| expr '(' exprs ')' {$$=new Call($1, $3);}
;

exprs
: exprs ',' expr {$$=$1; $$->push_back($3);}
| expr {$$=new std::vector<Expression*>(); $$->push_back($1);}
| /* blank */ {$$=new std::vector<Expression*>();}
;

cont_expr
: CONT '{' arg_defs '}' {$$=new Container($3);}
;

cont_eval
: expr '.' ID {$$=new ContainerEval($1, $3);}
;

arg_defs
: arg_defs ',' arg_def {$$=$1; $1->push_back($3);}
| arg_def {$$=new std::vector<dict*>(); $$->push_back($1);}

arg_def
: ID ':' expr {$$=new dict($1, $3);}
;

ids
: ids ',' ID {$$=$1; $$->push_back($3);}
| ID {$$=new std::vector<Identifier*>(); $$->push_back($1);}
| /* blank */ {$$=new std::vector<Identifier*>();}
;


arr_expr
: '[' exprs ']' {$$=new Array($2);}
;

value
: ID {$$=$1;}
| NUM {$$=$1;}
| STRING {$$=$1;}
| BOOL {$$=$1;}
;

%%

void yyerror(const char* e) {
    std::cout << "err[parser]: " << e << std::endl;
}