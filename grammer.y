program 
: root_declaration

root_declaration
: external_declaration
| root_declaration external_declaration
;

external_declaration
: function_declaration
| declaration
;

function_declaration
: declarator declaration_list compound_statment
| declarator compound_statment
;

declaration_list
: declaration
| declaration_list declaration
;


declaration
: declaration_specifiers ';'
| declaration_specifiers init_declarator_list ';'
;

declaration_specifiers
: type_specifier
| type_specifier declaration_specifiers
;

type_specifier:
: INT
| CHAR
;

init_declarator_list
: init_declarator
| init_declarator_list ',' init_declarator
;

init_declarator
: declarator
| declarator '=' initializer
;

declarator
: direct_declarator
;


direct_declarator
: IDENTIFIER
| '(' declarator ')'
;

initializer
: assign_expr
;

assign_expr
| unary_expr assign_oper assign_expr
;

assign_oper
: '='
;

unary_expr
: postfix_expr
;

postfix_expr
: primary_expr
;

primary_expr
: IDENTIFIER
| CONSTANT
| STRING_LITERAL
;
