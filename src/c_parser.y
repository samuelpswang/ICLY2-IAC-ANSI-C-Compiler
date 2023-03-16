%code requires{
#include "ast.hpp"
#include <cassert>

extern const Node* g_root;

int yylex(void);
void yyerror(const char*);
}

%union {
    Node* node;
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

%type <node> expression statement_list statement function declaration init_declarator constant_expression primary_expression
%type <node> unary_expression postfix_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression selection_statement iteration_statement jump_statement function_list 
%type <node> declaration_list argument_expression_list FOR for_loop_declaration
%type <string> CONSTANT IDENTIFIER INT type_specifier direct_declarator INC_OP DEC_OP declarator VOID DOUBLE LEFT_OP RIGHT_OP
%type <string> LE_OP GE_OP IF ELSE WHILE DO unary_operator RETURN

%start root

%%

root : function_list { 
        g_root = $1;
    }
    ;


type_specifier
    : INT { $$ = new std::string("int"); }
    | VOID { $$ =  new std::string("void"); }
    | DOUBLE { $$ = new std::string ("double");}


primary_expression
	: IDENTIFIER { $$ = new Identifier(*$1);}
	| CONSTANT {$$ = new Int(*$1);}
	| '(' expression ')' {$$ = $2;}
	;


postfix_expression
	: primary_expression { $$ = $1; }
    | postfix_expression INC_OP { $$ = new PostfixUnaryIncDecOp(*$2,$1); }
    | postfix_expression DEC_OP { $$ = new PostfixUnaryIncDecOp(*$2,$1); }
    | postfix_expression '(' ')' { $$ = new FunctionCall($1);}
	| postfix_expression '(' declaration_list ')'  { $$ = new FunctionCall($1,$3);}

argument_expression_list
	: declaration { $$ = new ArgumentList(new Argument($1->get_type(),$1->get_name())); }
	| argument_expression_list ',' declaration {$1->append_expr(new Argument($3->get_type(),$3->get_name()));  }
	;

unary_expression
	: postfix_expression { $$ = $1;}
	| INC_OP unary_expression { $$ = new PrefixUnaryIncDecOp(*$1,$2); }
	| DEC_OP unary_expression { $$ = new PrefixUnaryIncDecOp(*$1,$2); }
	| unary_operator unary_expression { if(*$1 == "-"){
                                            $$ = new NegOp($2);
                                        } 

                                        
                                    
                                    
                                    }
	;

unary_operator
	: '&' { $$ = new std::string("&"); }
	| '*' { $$ = new std::string("*"); }
	| '+' { $$ = new std::string("+"); }
	| '-' { $$ = new std::string("-"); }
	| '~' { $$ = new std::string("~"); }
	| '!' { $$ = new std::string("!"); }
	;



multiplicative_expression
	: unary_expression { $$ = $1 ;}
	| multiplicative_expression '*' primary_expression  { $$ = new MultOp($1,$3); }
	| multiplicative_expression '/' primary_expression  { $$ = new DivOp($1,$3); }
	| multiplicative_expression '%' primary_expression  { $$ = new ModOp($1,$3); }
	;

additive_expression
	: multiplicative_expression { $$ = $1;}
	| additive_expression '+' multiplicative_expression { $$ = new AddOp($1,$3); }
	| additive_expression '-' multiplicative_expression { $$ = new SubOp($1,$3); }
	;

shift_expression
	: additive_expression { $$ = $1 ;}
	| shift_expression LEFT_OP additive_expression { $$ = new LeftShift($1,$3) ;}
	| shift_expression RIGHT_OP additive_expression { $$ = new RightShift($1,$3) ;}
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression { $$ = new LessThanOp($1,$3);}
	| relational_expression '>' shift_expression { $$ = new MoreThanOp($1,$3);}
	| relational_expression LE_OP shift_expression { $$ = new LessEqual($1,$3); }
	| relational_expression GE_OP shift_expression { $$ = new MoreEqual($1,$3); }
	;

equality_expression
	: relational_expression { $$ = $1 ;}
	| equality_expression EQ_OP relational_expression { $$ = new EqualTo($1,$3); }
	| equality_expression NE_OP relational_expression { $$ = new NotEqualTo($1,$3); }
	;

and_expression
	: equality_expression { $$ = $1 ;}
	| and_expression '&' equality_expression { $$ = new BitwiseAnd($1,$3); }
	;

exclusive_or_expression
	: and_expression { $$ = $1 ;}
	| exclusive_or_expression '^' and_expression { $$ = new BitwiseXor($1,$3); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1 ;}
	| inclusive_or_expression '|' exclusive_or_expression { $$ = new BitwiseOr($1,$3); }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1 ; }
	| logical_and_expression AND_OP inclusive_or_expression { $$ = new LogicalAnd($1,$3); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1 ;}
	| logical_or_expression OR_OP logical_and_expression { $$ = new LogicalOr($1,$3); }
	;

conditional_expression
	: logical_or_expression { $$ = $1 ;}
	;

assignment_expression
	: conditional_expression { $$ = $1 ;}
	| unary_expression assignment_operator assignment_expression  { $$ = new AssignOp($1,$3);}
	;

expression
	: assignment_expression { $$ = $1 ;}
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression { $$ = $1 ;}
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

declarator
	: direct_declarator { $$ = $1 ;}
	;

direct_declarator
	: IDENTIFIER { $$ = $1;}
	;

function
    : type_specifier declarator '(' ')' '{' statement_list '}' { $$ = new Function(*$1, *$2, $6); }
	| type_specifier declarator '(' argument_expression_list ')'  '{' statement_list '}'    { $$ = new Function(*$1, *$2, $4, $7);}
    ;

function_list
    : function { $$ = new FunctionList($1); }
    | function_list function { $1->append_expr($2); }

statement_list
    : statement { $$ = new StatementList($1); }
    | statement_list statement { $1->append_statement($2); }
    ;

statement
	: declaration ';' { $$ = new Statement("declaration",$1) ;}
    | init_declarator ';' { $$ = new Statement("init_declarator",$1) ;}
    | expression ';' { $$ = new Statement("expression",$1) ;}
    | selection_statement { $$ = new Statement("",$1); }
    | iteration_statement { $$ = new Statement("",$1); }
    | jump_statement { $$ = new Statement("",$1); }
    | function  { $$ = new Statement("",$1); }
	;


declaration_list
	: declaration { $$ = new DeclarationList($1); }
	| declaration_list ',' declaration { $1->append_expr($3); }




declaration
    : type_specifier declarator  { $$ = new Declaration(*$1,*$2); }
    ;

init_declarator
    : type_specifier direct_declarator '=' constant_expression { $$ = new InitDeclaration(*$1,*$2,$4);}


selection_statement
	: IF '(' conditional_expression ')' '{' statement_list '}' { $$ = new If($3,$6); }
	| IF '(' conditional_expression ')' '{'statement_list '}' ELSE '{'statement'}' { $$ = new IfElse($3,$6,$10); }
	| IF '(' conditional_expression ')' '{''}' { $$ = new If($3, nullptr); }
	| IF '(' conditional_expression ')''{''}' ELSE '{''}' { $$ = new IfElse($3, nullptr,nullptr); }
	;

for_loop_declaration
	: init_declarator { $$ = $1; }
	| assignment_expression { $$ = $1; }

iteration_statement
	: WHILE '(' conditional_expression ')' '{' statement_list '}' { $$ = new While($3,$6); }
	| WHILE '(' conditional_expression ')' '{''}' { $$ = new While($3, nullptr); }
	| DO '{'statement_list'}' WHILE '(' conditional_expression ')' { $$ = new While($7,$3); }
	| FOR '(' for_loop_declaration ';' conditional_expression ';' expression ')' '{' statement_list'}' { $$ = new For($3,$5,$7,$10);}
	| FOR '(' for_loop_declaration ';' conditional_expression ')' '{' statement_list'}' { $$ = new For($3,$5,nullptr,$8); }
	| FOR '(' conditional_expression ';' expression ')' '{' statement_list'}' { $$ = new For(nullptr,$3,$5,$8); }
	| FOR '(' conditional_expression ')' '{' statement_list'}' { $$ = new For(nullptr,$3,nullptr,$6); }
	| FOR '(' for_loop_declaration ';' conditional_expression ';' expression ')' '{''}' { $$ = new For($3,$5,$7,nullptr); }	
	| FOR '(' for_loop_declaration ';' conditional_expression ')' '{''}' { $$ = new For($3,$5,nullptr,nullptr); }
	| FOR '(' conditional_expression ';' expression ')' '{''}' { $$ = new For(nullptr,$3,$5,nullptr); }																			
	| FOR '(' conditional_expression ')' '{' '}' { $$ = new For(nullptr,$3,nullptr,nullptr); }
	;


jump_statement
	: RETURN expression ';' { $$ = new Return($2); }
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
