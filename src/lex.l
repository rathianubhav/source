%{
#include <iostream>
#include "../include/node.h"
#include "parser.tab.h"


Oper getOper(const char* s) {
    switch(s[0]) {
        case '=': return EQ;
        case '!': return NE;
        case '>': return (s[1] == '=' ? GE : GT);
        case '<': return (s[1] == '=' ? LE : LT);
        default : exit(8);
    }
}

char* trim(char* str, char s) {
    char* toret = str;
    while(toret[0] == s) ++toret;
    for(int i=0; toret[i] != '\0'; i++) if(toret[i] == s) toret[i] = '\0';
    return toret;
}



%}

%option noyywrap
%option nounput
%option yylineno

%%

":=" return ASSIGN;
[+-] yylval.op = (yytext[0] == '+' ? ADD : SUB); return OPA;
[*/] yylval.op = (yytext[0] == '*' ? MUL : DIV); return OPM;
"." return '.';
"(" return '(';
")" return ')';
"{" return '{';
"}" return '}';
"[" return '[';
"]" return ']';
";" return ';';
":" return ':';
"," return ',';

[><]|([><!=]=) yylval.op = getOper(yytext); return COMP;

"int" return INT;
"float" return FLOAT;

"u8" return U8;
"u16" return U16;
"u32" return U32;
"u64" return U64;

"i8" return I8;
"i16" return I16;
"i32" return I32;
"i64" return I64;

"f32" return F32;
"f64" return F64;

"bool" return BOOL;
"func" return FUNC;
"let" return LET;
"ret" return RET;

"if" return IF;
"else" return ELSE;

[ \t\v\n\r\f] ;
[a-zA-Z_][a-zA-Z_0-9]* yylval.id = new Identifier(yytext); return ID;
([0-9]*[.])?[0-9]+ yylval.num = new Number(yytext); return NUM;
"#!".* ;
"/*"((\*+[^/*])|([^*]))*\**"*/" ;
"//".* ;
. std::cout << "err[lex]: unknow token starting with " << yytext << std::endl;

%%