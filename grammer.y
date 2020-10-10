program ::= {root_decl}+

root_decl ::= var_decl | func_decl

var_decl ::= TYPE {'*'}* IDEN ';'

func_decl ::= TYPE {'*'}* IDEN '(' parm_decl ')' '{' body_decl '}'

parm_decl ::= TYPE {'*'}* IDEN {',' parm_decl }*

body_decl ::= {var_decl}, {stmt}

stmt ::= if_stmt 
     |   while_stmt 
     |   '{' stmt '}' 
     |   'return' expr ';' 
     |   expr ';'
     
if_stmt ::= 'if' '(' expr ')' stmt [ 'else' stmt ]

while_stmt ::= 'while' '(' expr ')' stmt

expr ::= expr '+' term
     |   expr '-' term
     |   term
     
term ::= term '*' factor
     |   term '/' factor
     |   factor
     
factor ::= ( expr )
       |   NUM
