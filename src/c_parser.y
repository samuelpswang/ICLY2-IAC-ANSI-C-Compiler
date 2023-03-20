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

%token IDENTIFIER INT_VALUE FLOAT_VALUE STRING_LITERAL CHAR_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID HEX
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> expression statement_list statement function declaration init_declarator constant_expression primary_expression pointer pointer_assignment
%type <node> unary_expression postfix_expression multiplicative_expression additive_expression shift_expression relational_expression struct_declaration_list struct_declaration
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression struct struct_init_variable
%type <node> conditional_expression assignment_expression selection_statement iteration_statement jump_statement globals_list enumerator_list enum_specifier
%type <node> declaration_list argument_expression_list FOR for_loop_declaration constant_expression_list argument array_declaration enum_item globals
%type <string> INT_VALUE FLOAT_VALUE IDENTIFIER INT type_specifier direct_declarator INC_OP DEC_OP declarator VOID DOUBLE LEFT_OP RIGHT_OP
%type <string> LE_OP GE_OP IF ELSE WHILE DO unary_operator RETURN FLOAT STRING_LITERAL CHAR_LITERAL CHAR ENUM CONTINUE BREAK UNSIGNED STRUCT
%start root

%%

root : globals_list { 
        g_root = $1;
    }
    ;

globals_list
	: globals { $$ = new Root($1); }
	| globals_list globals { $1->append_expr($2); }


globals
	: function { $$ = $1; }
	| type_specifier declarator { $$ = new GlobalDeclaration(*$1, *$2); }
	| type_specifier direct_declarator '=' constant_expression { $$ = new GlobalInitDeclaration(*$1,*$2,$4);}
	| type_specifier direct_declarator '[' INT_VALUE ']' ';' { $$ = new GlobalArrayDeclarator(*$1,*$2, *$4); }
	| enum_specifier { $$ = $1; }
	| struct { $$ = $1; }


type_specifier
    : INT { $$ = new std::string("int"); }
    | VOID { $$ =  new std::string("void"); }
    | DOUBLE { $$ = new std::string ("double"); }
	/* | FLOAT { $$ = new std::string ("float"); } */
	| CHAR { $$ = new std::string("char"); }
	| UNSIGNED { $$ = new std::string("unsigned"); }


enum_specifier
	: ENUM '{' enumerator_list '}' ';' { $$ = new EnumDeclarator("", $3); }
	| ENUM IDENTIFIER '{' enumerator_list '}' ';' { $$ = new EnumDeclarator(*$2, $4); }
	;

enumerator_list
	: enum_item { $$ = new EnumList($1); }
	| enumerator_list ',' enum_item { $1->append_expr($3); }

enum_item
	: IDENTIFIER { $$ = new EnumItem(*$1);}
	| IDENTIFIER '=' INT_VALUE { $$ = new EnumItemWithValue(*$1, *$3); }


primary_expression
	: IDENTIFIER { $$ = new Identifier(*$1);}
	| INT_VALUE {$$ = new Int(*$1); }
	/* | FLOAT_VALUE { $$ = new Float(*$1); } */
	| '(' expression ')' {$$ = $2;}
	| CHAR_LITERAL { $$ = new Char(*$1); }
	| STRING_LITERAL { $$ = new String(*$1); }
	| IDENTIFIER '.' IDENTIFIER { $$ = new Identifier(*$1+"."+*$3); }
	;

constant_expression_list
	: constant_expression { $$ = new PrimaryExpressionList($1); }
	| constant_expression_list ',' constant_expression { $1->append_expr($3); }



postfix_expression
	: primary_expression { $$ = $1; }
    | postfix_expression INC_OP { $$ = new PostfixUnaryIncDecOp(*$2,$1); }
    | postfix_expression DEC_OP { $$ = new PostfixUnaryIncDecOp(*$2,$1); }
	| postfix_expression '[' expression ']'  { $$ = new ArrayAccessor($1->get_name(),$3);}
    | postfix_expression '(' ')' { $$ = new FunctionCall($1);}
	| postfix_expression '(' constant_expression_list ')'   { $$ = new FunctionCall($1, $3);}
	

argument_expression_list
	: argument { $$ = new ArgumentList(new Argument($1->get_type(),$1->get_name())); }
	| argument_expression_list ',' argument {$1->append_expr(new Argument($3->get_type(),$3->get_name()));  }
	;

argument
    : type_specifier declarator  { $$ = new Argument(*$1,*$2); }
    ;

unary_expression
	: postfix_expression { $$ = $1;}
	| INC_OP unary_expression { $$ = new PrefixUnaryIncDecOp(*$1,$2); }
	| DEC_OP unary_expression { $$ = new PrefixUnaryIncDecOp(*$1,$2); }
	| unary_operator unary_expression { if(*$1 == "-"){
                                            $$ = new NegOp($2);
                                        } 
										if(*$1 == "&"){
											$$ = new AddressOfOperator($2);
										}
										if(*$1 == "*"){
											$$ = new DereferenceOperator($2);
										}
										
										}
	| SIZEOF unary_expression { $$ = new SizeOf($2); }
	| SIZEOF '(' type_specifier ')' { $$ = new SizeOf(*$3); }
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

array_declaration
	: type_specifier direct_declarator '[' INT_VALUE ']' ';' { $$ = new ArrayDeclarator(*$1,*$2, *$4); }


direct_declarator
	: IDENTIFIER { $$ = $1;}
	;

pointer_assignment
	: type_specifier '*' primary_expression assignment_operator assignment_expression ';' { $$ = new PointerAssignOp(new DereferenceOperator($3),$5); } 

function
    : type_specifier declarator '(' ')' '{' statement_list '}' { $$ = new Function(*$1, *$2,nullptr, $6); }
	| type_specifier declarator '(' argument_expression_list ')'  '{' statement_list '}' { $$ = new Function(*$1, *$2, $4, $7);}
	| type_specifier declarator '(' ')' '{' '}' { $$ = new Function(*$1, *$2, nullptr, nullptr); }
	| type_specifier declarator '(' ')' ';' { $$ = new Function(*$1, *$2, nullptr,nullptr); }
	| type_specifier declarator '(' argument_expression_list ')' ';' { $$ = new Function(*$1, *$2,$4,nullptr); }
    ;

statement_list
    : statement { $$ = new StatementList($1); }
    | statement_list statement { $1->append_statement($2); }
    ;

statement
	: declaration ';' { $$ = new Statement("declaration",$1) ;}
    | init_declarator ';' { $$ = new Statement("init_declarator",$1) ;}
    | expression ';' { $$ = new Statement("expression",$1) ;}
    | selection_statement { $$ = new Statement("selection",$1); }
    | iteration_statement { $$ = new Statement("iteration",$1); }
    | jump_statement { $$ = new Statement("jump",$1); }
    | function  { $$ = new Statement("function",$1); }
	| array_declaration { $$ = new Statement("array",$1); }
	| pointer_assignment { $$ = new Statement("pointer_assignment",$1); }
	| '{' statement_list '}' { $$ = new Statement("scoped",$2); }
	| struct_init_variable { $$ = new Statement("struct", $1); }
	;


declaration_list
	: declaration { $$ = new DeclarationList($1); }
	| declaration_list ',' declaration { $1->append_expr($3); }
	


struct
	: STRUCT declarator '{' struct_declaration_list '}' ';' { $$ = new StructDefinition(*$2, $4); }
	;

struct_declaration_list
	: struct_declaration {$$ = new StructDeclarationList($1); }
	| struct_declaration_list struct_declaration {$1->append_expr($2); }

struct_declaration
	: type_specifier declarator ';'  {$$ = new StructMemberDeclaration(*$1,*$2); }


declaration
    : type_specifier declarator  { $$ = new Declaration(*$1,*$2); }
    ;

struct_init_variable
	: STRUCT IDENTIFIER IDENTIFIER ';' { $$ = new StructInitVariable(*$2, *$3); std::cout<<"Defining Strcut"<<std::endl; }

init_declarator
    : type_specifier declarator '=' constant_expression { $$ = new InitDeclaration(*$1,*$2,$4);}


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
	| CONTINUE ';' { $$ = new Continue(); } 
	| BREAK ';' { $$ = new Break(); }
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
