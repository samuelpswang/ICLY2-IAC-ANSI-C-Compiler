%option noyywrap

%{
extern "C" int fileno(FILE *stream);
#include <iostream>
#include <string>
#include "ast.hpp"
#include "c_parser.tab.hpp"

%}

D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%%

"auto"		    	{ yylval.string = new std::string(yytext); return AUTO; }
"break"		    	{ yylval.string = new std::string(yytext); return BREAK; }
"case"			    { yylval.string = new std::string(yytext); return CASE; }
"char"		    	{ yylval.string = new std::string(yytext); return CHAR; }
"const"		    	{ yylval.string = new std::string(yytext); return CONST; }
"continue"	    	{ yylval.string = new std::string(yytext); return CONTINUE; }
"default"	    	{ yylval.string = new std::string(yytext); return DEFAULT; }
"do"		       	{ yylval.string = new std::string(yytext); return DO; }
"double"	    	{ yylval.string = new std::string(yytext); return DOUBLE; }
"else"		    	{ yylval.string = new std::string(yytext); return ELSE; }
"enum"		    	{ yylval.string = new std::string(yytext); return ENUM; }
"extern"	    	{ yylval.string = new std::string(yytext); return EXTERN; }
"float"		    	{ yylval.string = new std::string(yytext); return FLOAT; }
"for"		    	{ yylval.string = new std::string(yytext); return FOR; }
"goto"		    	{ yylval.string = new std::string(yytext); return GOTO; }
"if"		    	{ yylval.string = new std::string(yytext); return IF; }
"int"		    	{ yylval.string = new std::string(yytext); return INT;  }
"long"		    	{ yylval.string = new std::string(yytext); return LONG; }
"register"	    	{ yylval.string = new std::string(yytext); return REGISTER; }
"return"	    	{ yylval.string = new std::string(yytext); return RETURN; }
"short"			    { yylval.string = new std::string(yytext); return SHORT; }
"signed"	    	{ yylval.string = new std::string(yytext); return SIGNED; }
"sizeof"	    	{ yylval.string = new std::string(yytext); return SIZEOF; }
"static"	    	{ yylval.string = new std::string(yytext); return STATIC; }
"struct"		    { yylval.string = new std::string(yytext); return STRUCT; }
"switch"	    	{ yylval.string = new std::string(yytext); return SWITCH; }
"typedef"		    { yylval.string = new std::string(yytext); return TYPEDEF; }
"union"			    { yylval.string = new std::string(yytext); return UNION; }
"unsigned"		    { yylval.string = new std::string(yytext); return UNSIGNED; }
"void"			    { yylval.string = new std::string(yytext); return VOID; }
"volatile"		    { yylval.string = new std::string(yytext); return VOLATILE; }
"while"			    { yylval.string = new std::string(yytext); return WHILE; }

0[xX]{H}+{IS}?		{ yylval.string = new std::string(yytext); return CONSTANT; }
0{D}+{IS}?		    { yylval.string = new std::string(yytext); return CONSTANT; }
{D}+{IS}?		    { yylval.string = new std::string(yytext); return CONSTANT; }
L?'(\\.|[^\\'])+'	{ yylval.string = new std::string(yytext); return CONSTANT; }

{D}+[{E}{FS}]?		{ yylval.string = new std::string(yytext); return CONSTANT; }
{D}*"."{D}+({E})?{FS}?	{ yylval.string = new std::string(yytext); return CONSTANT; }
{D}+"."{D}*({E})?{FS}?	{ yylval.string = new std::string(yytext); return CONSTANT; }

L?\"(\\.|[^\\"])*\"	{ return(STRING_LITERAL); }

"..."		    	{ yylval.string = new std::string(yytext); return(ELLIPSIS); }
">>="		    	{ yylval.string = new std::string(yytext); return(RIGHT_ASSIGN); }
"<<="		    	{ yylval.string = new std::string(yytext); return(LEFT_ASSIGN); }
"+="	    		{ yylval.string = new std::string(yytext); return(ADD_ASSIGN); }
"-="		    	{ yylval.string = new std::string(yytext); return(SUB_ASSIGN); }
"*="		    	{ yylval.string = new std::string(yytext); return(MUL_ASSIGN); }
"/="	    		{ yylval.string = new std::string(yytext); return(DIV_ASSIGN); }
"%="		    	{ yylval.string = new std::string(yytext); return(MOD_ASSIGN); }
"&="	    		{ yylval.string = new std::string(yytext); return(AND_ASSIGN); }
"^="		    	{ yylval.string = new std::string(yytext); return(XOR_ASSIGN); }
"|="		    	{ yylval.string = new std::string(yytext); return(OR_ASSIGN); }
">>"		    	{ yylval.string = new std::string(yytext); return(RIGHT_OP); }
"<<"		    	{ yylval.string = new std::string(yytext); return(LEFT_OP); }
"++"		    	{ yylval.string = new std::string(yytext); return(INC_OP); }
"--"		    	{ yylval.string = new std::string(yytext); return(DEC_OP); }
"->"		    	{ yylval.string = new std::string(yytext); return(PTR_OP); }
"&&"		    	{ yylval.string = new std::string(yytext); return(AND_OP); }
"||"	     		{ yylval.string = new std::string(yytext); return(OR_OP); }
"<="	    		{ yylval.string = new std::string(yytext); return(LE_OP); }
">="	    		{ yylval.string = new std::string(yytext); return(GE_OP); }
"=="	    		{ yylval.string = new std::string(yytext); return(EQ_OP); }
"!="	    		{ yylval.string = new std::string(yytext); return(NE_OP); }
";"		        	{  return(';'); }
("{"|"<%")	    	{  return('{'); }
("}"|"%>")	    	{  return('}'); }
","		    	    {  return(','); }
":"		    	    {  return(':'); }
"="		           	{  return('='); }
"("		        	{  return('('); }
")"		        	{  return(')'); }
("["|"<:")	    	{  return('['); }
("]"|":>")	    	{  return(']'); }
"."			        {  return('.'); }
"&"		        	{  return('&'); }
"!"			        {  return('!'); }
"~"			        {  return('~'); }
"-"			        {  return('-'); }
"+"		        	{  return('+'); }
"*"		        	{  return('*'); }
"/"		        	{  return('/'); }
"%"		        	{  return('%'); }
"<"		        	{  return('<'); }
">"	        		{  return('>'); }
"^"		        	{  return('^'); }
"|"	        		{  return('|'); }
"?"		        	{  return('?'); }

{L}+                { yylval.string = new std::string(yytext); return IDENTIFIER; }

[ \t\v\n\f]		    {}
.			        { fprintf(stderr, "Invalid token: %s\n", yytext); exit(1); }

%%

void yyerror(char const *s) {
    fprintf(stderr, "Parse error: %s\n STOP", s);
    exit(1);
}
