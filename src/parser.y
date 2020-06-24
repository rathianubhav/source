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

%token FUNC RET LET
%token IF ELSE
%token ASSIGN
%token INT BOOL FLOAT
%token I8 I16 I32 I64 U8 U16 U32 U64 F32 F64

%type<toplevel> top_level proto func_def
%type<arr> program top_levels
%type<block> block
%type<arrS> stmts args_list
%type<arrE> exprs
%type<stmt> stmt ret_stmt arg let_stmt cond_stmt expr_stmt
%type<expr> expr val call_expr
%type<type> type sub_type
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
| {$$=new std::vector<Statment*>();}
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
| let_stmt
| cond_stmt
| expr_stmt
;


ret_stmt
: RET expr ';' {$$=new Return($2);}
;


let_stmt
: LET ID type ';' {$$=new Let($2,$3);}
| ID type ASSIGN expr ';' {$$=new Let($1, $2, $4);}
;

cond_stmt
: IF expr block {$$=new Condition($2, $3);}
| IF expr block ELSE block {$$=new Condition($2, $3, $5);}
;

expr_stmt
: expr ';' {$$=new ExpressionStatment($1);}
;

expr
: expr OPA expr {$$=new Arithmetic($1, $2, $3);}
| expr OPM expr {$$=new Arithmetic($1, $2, $3);}
| call_expr
| val
;

call_expr
: ID '(' exprs ')' {$$=new Call($1, $3);}
;


exprs
: exprs ',' expr {$$=$1; $$->push_back($3);}
| expr {$$=new std::vector<Expression*>(); $$->push_back($1);}
| {$$=new std::vector<Expression*>();}
;

val
: ID {$$=$1;}
| NUM {$$=$1;}
;

type
: sub_type
| INT {$$=new IntType();}
| FLOAT {$$=new FloatType();}
;

sub_type
: U8 {$$=new IntType(8, 0);}
| U16 {$$=new IntType(16, 0);}
| U32 {$$=new IntType(32, 0);}
| U64 {$$=new IntType(64, 0);}
| I8 {$$=new IntType(8, 1);}
| I16 {$$=new IntType(16, 1);}
| I32 {$$=new IntType(32, 1);}
| I64 {$$=new IntType(64, 1);}
| BOOL {$$=new IntType(1);}
| F32 {$$=new FloatType(32, 1);}
| F64 {$$=new FloatType(64, 1);}
;

%%
extern int yylineno;
void yyerror(const char* e) {
    std::cout << "Error: " << e << "\nLine No: " << yylineno << std::endl;
}