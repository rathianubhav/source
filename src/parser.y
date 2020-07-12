%code requires {
#include "../include/node.h"

int yylex(void);
int yylex_destroy();

using namespace std;

extern vector<Statment*> *tree;
extern bool interactive;

void yyerror(const char* e);

}

%define parse.error verbose

%union {
    Identifier *id;
    Expression *expr;
    String *str;
    Number *num;
    Operator op;

    Statment *stmt;
    Block *block;

    
    ContainerData *deft;
    vector<ContainerData*> *arrC;
    vector<Statment*>    *arr;
    vector<Expression*>  *arrE;
    vector<Identifier*>  *arrI;

};

%left<op> BOP
%left<op> COMP
%left<op> OPA
%left<op> OPM


%token<id> ID
%token<expr> NUM BOOL STRING
%token ASSIGN NOTTOK
%token LET IN
%token IF ELSE FOR FUNC CONT
%token PRINT USE DEBUG CMOD
%token NULL_T BREAK CONTINUE RETURN

%type<expr> value expr
%type<expr> math_expr arr_expr method_expr call_expr cont_expr

%type<stmt> stmt block

%type<stmt> assign_stmt print_stmt expr_stmt cond_stmt use_stmt
%type<stmt> loop_stmt debug_stmt

%type<arr> program stmts
%type<arrE> exprs
%type<arrI> ids
%type<arrC> defs
%type<deft> def

%start program

%%

program
: stmts {$$=$1; tree=$$; if(interactive) YYACCEPT;}
;

stmt
: assign_stmt
| use_stmt
| expr_stmt
| cond_stmt
| loop_stmt
| print_stmt
| debug_stmt
| BREAK ';' {$$=new Break();}
| CONTINUE ';' {$$=new Continue();}
| RETURN expr ';' {$$=new Return(*$2);}
;

assign_stmt
: LET ID ASSIGN expr ';' {$$=new Let(*$2, *$4);}
| ID ASSIGN expr ';' {$$=new Assign(*$1, *$3);}
;

debug_stmt
: DEBUG ID ';' {$$=new Debug(*$2);}
;

use_stmt
: USE ID ';' {$$=new Use(*$2);}
;

cond_stmt
: IF expr block {$$=new Condition(*$2, *$<block>3);}
| IF expr block ELSE block {$$=new Condition(*$2, *$<block>3, $<block>5);}
;

expr_stmt
: expr ';' {$$=new ExprStatment($1);}
;

print_stmt
: PRINT '(' exprs ')' ';' {$$=new Print($3);}
;


loop_stmt
: FOR expr block {$$=new Loop(*$2, *$<block>3);}
| FOR assign_stmt expr ';' assign_stmt block {$$=new Loop(*$2, *$3, *$5, *$<block>6);}
| FOR ID IN expr block {$$=new Loop(*$2, *$4, *$<block>5);}
;

block
: '{' stmts '}' {$$=new Block($2);}
| {$$=new Block(nullptr);}
;

stmts
: stmts stmt {$$=$1; $$->push_back($2);}
| {$$=new std::vector<Statment*>();}
;

expr
: math_expr
| arr_expr
| value
| method_expr
| cont_expr
| call_expr
| CMOD '(' exprs ')' {$$=new Cmod($3);}
| '(' expr ')' {$$=$2;}
| NULL_T {$$=new Null();}
;

math_expr
: expr COMP expr {$$=new Compare(*$1, $2, *$3);}
| expr BOP expr {$$=new Logical(*$1, $2, *$3);}
| expr OPA expr {$$=new Arithmetic(*$1, $2, *$3);}
| expr OPM expr {$$=new Arithmetic(*$1, $2, *$3);}
| NOTTOK expr {$$=new Negation(*$2);}
;

method_expr
: FUNC ids block {$$=new Method($2, $<block>3);}
;

cont_expr
: CONT '{' defs '}' {$$=new Container(*$3);}
;


defs
: defs ',' def {$$=$1; $$->push_back($3);}
| def {$$=new vector<ContainerData*>(); $$->push_back($1);}
;

def
: ID ':' expr {$$=new ContainerData($1, $3);}
;

call_expr
: ID '(' exprs ')' {$$=new Call(*$1, *$3);}
;

exprs
: exprs ',' expr {$$=$1; $$->push_back($3);}
| expr {$$=new std::vector<Expression*>(); $$->push_back($1);}
| /* blank */ {$$=new std::vector<Expression*>();}
;


arr_expr
: '[' exprs ']' {$$=new Array($2);}
| ID '[' expr ']' {$$=new Access(*$1, *$3);}
;

ids 
: ids ',' ID {$$=$1; $$->push_back($3);}
| ID {$$=new vector<Identifier*>(); $$->push_back($1);}
| /* blank */ {$$=new vector<Identifier*>();}
;

value
: ID {$$=$1;}
| NUM {$$=$1;}
| STRING {$$=$1;}
| BOOL {$$=$1;}
;

%%

void
yyerror(const char* e)
{
    int yylineno;
    std::cout << "Error:[" << yylineno << "] " << e << std::endl;
}