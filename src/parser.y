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

%token FUNC ASSIGN LET FOR IF ELSE PRINT PRINTLN TYPEOF USE


%type<expr> value expr
%type<expr> math_expr func_expr typeof_expr
%type<stmt> stmt
%type<stmt> assign_stmt condit_stmt loop_stmt expr_stmt print_stmt use_stmt 
%type<block> block

%type<arr> program stmts
%type<arrE> exprs
%type<arrI> ids

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
| typeof_expr
| value
| '(' expr ')' {$$=$2;}
| '[' exprs ']' {$$=new Array($2);}
| ID '[' expr ']' {$$=new Access($1, $3);}
;


math_expr
: expr COMP expr {$$=new Compare($1, $2, $3);}
| expr OPA expr {$$=new Arithmetic($1, $2, $3);}
| expr OPM expr {$$=new Arithmetic($1, $2, $3);}
;

func_expr
: FUNC ids block {$$=new Method($2, $3);}
| ID '(' exprs ')' {$$=new Call($1, $3);}
;

exprs
: exprs ',' expr {$$=$1; $$->push_back($3);}
| expr {$$=new std::vector<Expression*>(); $$->push_back($1);}
| /* blank */ {$$=new std::vector<Expression*>();}
;

ids
: ids ',' ID {$$=$1; $$->push_back($3);}
| ID {$$=new std::vector<Identifier*>(); $$->push_back($1);}
| /* blank */ {$$=new std::vector<Identifier*>();}
;

typeof_expr
: TYPEOF '(' expr ')' {$$=new Typeof($3);}
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