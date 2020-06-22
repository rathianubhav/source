%code requires {

#include "../include/node.h"
using namespace source;
int yylex(void);
int yylex_destroy();

extern std::vector<TopLevel*> *tree;

void yyerror(const char* e);

}

%define parse.error verbose

%union {
    Oper op;
    Identifier* id;
    Number* num;
    Expression* expr;
    TopLevel* toplevel;
    Block* block;
    Statment* stmt;
    std::vector<Expression*> *arrE;
    std::vector<Statment*> *arrS;
    std::vector<TopLevel*> *arr;
    DataType* type;
};

%left<op> BOP
%left<op> NOTTOK
%left<op> COMP
%left<op> OPA
%left<op> OPM
%right POSNEG

%token<id> ID
%token<num> NUM

%token FUNC EXTERN RET

%token INT BOOL
%token I8 I16 I32 I64 U8 U16 U32 U64

%type<toplevel> top_level proto func_def
%type<arr> program top_levels
%type<block> block
%type<arrS> stmts args_list
%type<arrE> exprs
%type<stmt> stmt ret_stmt call_stmt arg
%type<expr> expr val
%type<type> type int_type
%start program

%%

program
: top_levels {$$=$1; tree=$$;}
;

top_levels
: top_levels top_level {$$=$1; $$->push_back($2);}
| {$$=new std::vector<TopLevel*>();}
;

top_level
: proto ';'
| func_def
;

func_def
: proto block {$$=new Method($1, $2);}
;

proto
: FUNC ID '(' args_list ')' type {$$=new Prototype($2, $4, $6);}
;

args_list
: args_list ',' arg {$$=$1; $$->push_back($3);}
| arg {$$=new std::vector<Statment*>(); $$->push_back($1);}
;

arg
: ID type {$$=new Argument($1, $2);}
;

block
: '{' stmts '}' {$$=new Block($2);}
;

stmts
: stmts stmt {$$=$1; $$->push_back($2);}
| {$$=new std::vector<Statment*>();}
;


stmt
: ret_stmt
| call_stmt
;


ret_stmt
: RET expr ';' {$$=new Return($2);}
;

call_stmt
: ID '(' exprs ')' {$$=new Call($1, $3);}
;

expr
: expr OPA expr {$$=new Arithmetic($1, $2, $3);}
| expr OPM expr {$$=new Arithmetic($1, $2, $3);}
| val
;


exprs
: exprs expr {$$=$1; $$->push_back($2);}
| {$$=new std::vector<Expression*>();}
;

val
: ID {$$=$1;}
| NUM {$$=$1;}
;

type
: int_type
| INT {$$=new IntType();}
;

int_type
: U8 {$$=new IntType(8, 0);}
| U16 {$$=new IntType(16, 0);}
| U32 {$$=new IntType(32, 0);}
| U64 {$$=new IntType(64, 0);}
| I8 {$$=new IntType(8, 1);}
| I16 {$$=new IntType(16, 1);}
| I32 {$$=new IntType(32, 1);}
| I64 {$$=new IntType(64, 1);}
| BOOL {$$=new IntType(1);}
;

%%
extern int yylineno;
void yyerror(const char* e) {
    std::cout << "Error: " << e << "\nLine No: " << yylineno << std::endl;
}