%{
#include "ast.hpp"
#include <cassert>

extern const Node* g_root;

int yylex(void);
void yyerror(const char*);
%}

%union {
    const Node* node;
	std::string* string;
}

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> expression statement const
%type <string> CONSTANT IDENTIFIER

%start root

%%

root : statement { 
        g_root = new Statement($1);
    }
    ;

statement 
    : expression '=' const ';' {
        $$ = new AssignOp($1, $3);
    }
    ;

expression 
    : INT IDENTIFIER {
        $$ = new IntExpr(*$2)
    }
    ;

const 
    : CONSTANT {  
        $$ = new Int(*$1); 
    }
    ;

%%

/* #include <stdio.h>

extern char yytext[];
extern int column; */

/* yyerror(s)
char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
} */

const Node* g_root;

const Node* parse_ast() {
    g_root = 0;
    yyparse();
    return g_root;
}
