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

char* trim(char* str) {
    char* toret = str;
    while(toret[0] == '"') ++toret;
    for(int i=0; toret[i] != '\0'; i++) if(toret[i] == '"') toret[i] = '\0';
    return toret;
}


%}

%option noyywrap
%option nounput

%%

[+-] yylval.op = (yytext[0] == '+' ? ADD : SUB); return OPA;
[*/] yylval.op = (yytext[0] == '*' ? MUL : DIV); return OPM;
and|or yylval.op = (yytext[0] == 'a' ? AND : OR); return BOP;

":=" return ASSIGN;
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
true|false yylval.expr = new Bool(yytext[0] == 't'); return BOOL;

"use" return USE;
"func" return FUNC;
"cont" return CONT;
"let" return LET;
"if" return IF;
"else" return ELSE;
"for" return FOR;
"print" return PRINT;
"println" return PRINTLN;
"typeof" return TYPEOF;
"null" return SNULL;
"in" return IN;

[ \t\v\n\r\f] ;
[a-zA-Z_][a-zA-Z_0-9]* yylval.id = new Identifier(yytext); return ID;
([0-9]*[.])?[0-9]+ yylval.num = new Number(yytext); return NUM;
["][^"]*["] yylval.str = new String(trim(yytext)); return STRING;

. std::cout << "err[lex]: unknow token starting with " << yytext << std::endl;

%%