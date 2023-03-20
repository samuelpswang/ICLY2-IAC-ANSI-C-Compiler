%option noyywrap

%{
extern "C" int fileno(FILE *stream);
#include <iostream>
#include <string>
#include "ast.hpp"
#include "c_parser.tab.hpp"

void yyprint(char const* s, const std::string& base) {
    fprintf(stderr, "%s: %s\n", base.c_str(), s);
}

%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%%

"auto"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return AUTO; }
"break"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return BREAK; }
"case"			    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return CASE; }
"char"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return CHAR; }
"const"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return CONST; }
"continue"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return CONTINUE; }
"default"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return DEFAULT; }
"do"		       	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return DO; }
"double"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return DOUBLE; }
"else"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return ELSE; }
"enum"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return ENUM; }
"extern"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return EXTERN; }
"float"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return FLOAT; }
"for"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return FOR; }
"goto"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return GOTO; }
"if"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return IF; }
"int"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return INT;  }
"long"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return LONG; }
"register"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return REGISTER; }
"return"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return RETURN; }
"short"			    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return SHORT; }
"signed"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return SIGNED; }
"sizeof"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return SIZEOF; }
"static"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return STATIC; }
"struct"		    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return STRUCT; }
"switch"	    	{ yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return SWITCH; }
"typedef"		    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return TYPEDEF; }
"union"			    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return UNION; }
"unsigned"		    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return UNSIGNED; }
"void"			    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return VOID; }
"volatile"		    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return VOLATILE; }
"while"			    { yylval.string = new std::string(yytext); yyprint(yytext, "keyword"); return WHILE; }

0[xX]{H}+{IS}?		{ yylval.string = new std::string(yytext); yyprint(yytext, "constant"); return HEX; }
L?'(\\.|[^\\'])+'	{ yylval.string = new std::string(yytext); yyprint(yytext, "char literal"); return CHAR_LITERAL; }

{D}+[{E}{FS}]?		{ yylval.string = new std::string(yytext); yyprint(yytext, "integer"); return INT_VALUE; }
{D}*"."{D}+({E})?{FS}?	{ yylval.string = new std::string(yytext); yyprint(yytext, "constant"); return FLOAT_VALUE; }

L?\"(\\.|[^\\"])*\"	{ yylval.string = new std::string(yytext); yyprint(yytext, "string literal"); return STRING_LITERAL; }

"..."		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return ELLIPSIS; }
">>="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return RIGHT_ASSIGN; }
"<<="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return LEFT_ASSIGN; }
"+="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return ADD_ASSIGN; }
"-="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return SUB_ASSIGN; }
"*="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return MUL_ASSIGN; }
"/="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return DIV_ASSIGN; }
"%="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return MOD_ASSIGN; }
"&="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return AND_ASSIGN; }
"^="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return XOR_ASSIGN; }
"|="		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return OR_ASSIGN; }
">>"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return RIGHT_OP; }
"<<"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return LEFT_OP; }
"++"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return INC_OP; }
"--"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return DEC_OP; }
"->"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return PTR_OP; }
"&&"		    	{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return AND_OP; }
"||"	     		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return OR_OP; }
"<="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return LE_OP; }
">="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return GE_OP; }
"=="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return EQ_OP; }
"!="	    		{ yylval.string = new std::string(yytext); yyprint(yytext, "operator"); return NE_OP; }
";"		        	{ yyprint(yytext, "symbol"); return ';'; }
("{"|"<%")	    	{ yyprint(yytext, "symbol"); return '{'; }
("}"|"%>")	    	{ yyprint(yytext, "symbol"); return '}'; }
","		    	    { yyprint(yytext, "symbol"); return ','; }
":"		    	    { yyprint(yytext, "symbol"); return ':'; }
"="		           	{ yyprint(yytext, "symbol"); return '='; }
"("		        	{ yyprint(yytext, "symbol"); return '('; }
")"		        	{ yyprint(yytext, "symbol"); return ')'; }
("["|"<:")	    	{ yyprint(yytext, "symbol"); return '['; }
("]"|":>")	    	{ yyprint(yytext, "symbol"); return ']'; }
"."			        { yyprint(yytext, "symbol"); return '.'; }
"&"		        	{ yyprint(yytext, "symbol"); return '&'; }
"!"			        { yyprint(yytext, "symbol"); return '!'; }
"~"			        { yyprint(yytext, "symbol"); return '~'; }
"-"			        { yyprint(yytext, "symbol"); return '-'; }
"+"		        	{ yyprint(yytext, "symbol"); return '+'; }
"*"		        	{ yyprint(yytext, "symbol"); return '*'; }
"/"		        	{ yyprint(yytext, "symbol"); return '/'; }
"%"		        	{ yyprint(yytext, "symbol"); return '%'; }
"<"		        	{ yyprint(yytext, "symbol"); return '<'; }
">"	        		{ yyprint(yytext, "symbol"); return '>'; }
"^"		        	{ yyprint(yytext, "symbol"); return '^'; }
"|"	        		{ yyprint(yytext, "symbol"); return '|'; }
"?"		        	{ yyprint(yytext, "symbol"); return '?'; }

{L}+                { yylval.string = new std::string(yytext); yyprint(yytext, "identifier"); return IDENTIFIER; }

[ \t\v\n\f\r]		    {}
.			        { yyprint(yytext, "invalid token"); exit(1);}

%%

void yyerror(char const *s) {
    fprintf(stderr, "Parse error: %s\n STOP", s);
    exit(1);
}
