%{
		#include "stdio.h"
		#include "stdlib.h"
		#include "gramtree_v1.cpp.h"
		#include "gramtree_v1.tab.h"

 		int yycolumn = 1;
 		#define YY_USER_ACTION \
      yylloc.first_line = yylloc.last_line = yylineno; \
      yylloc.first_column = yycolumn; \
      yylloc.last_column = yycolumn + yyleng - 1; \
      yycolumn += yyleng;
%}

%option yylineno

TYPE int|float|char|void
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
PLUS \+
MINUS -
MUL \*
DIV \/
INTEGER [+-]?[0-9]+
FLOAT   [+-]?([0-9]*\.[0-9]+|[0-9]+\.)
CHAR \'[a-z_A-Z]\'
ID [a-z_A-Z][a-zA-Z_0-9]*
SPACE [ \t\r]*
EOL \n
SEMI ;
SINGLEQUOTE \'
COMMA ,
SPDOT    "."
ASSIGNOP =
GRAET >
LESS <
GASSIGN ">="
LESSASSIGN <=
DOUBLEASSIGN ==
NOTASSIGN "!="
PLUSASSIGN \+=
SUBASSIGN -=
DIVASSIGN \/=
MULASSIGN \*=
DOUBLEPLUS "++"
DOUBLESUB  "--"
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
COMMENT         (\/\/.*)|(\/\*((\*[^\/]?)|[^\*]*)*\*\/)
AERROR .
%%
int|float|char|void {printf("(%s,TYPE)\n",yytext);yylval.a=createNode("TYPE",0,yylineno);return TYPE;}
{SPACE} {}
{STRUCT} {printf("(%s,STRUCT)\n",yytext);yylval.a=createNode("STRUCT",0,yylineno);return STRUCT;}
{RETURN} {printf("(%s,RETURN)\n",yytext);yylval.a=createNode("RETURN",0,yylineno);return RETURN;}
{IF} {printf("(%s,IF)\n",yytext);yylval.a=createNode("IF",0,yylineno);return IF;}
{ELSE} {printf("(%s,ELSE)\n",yytext);yylval.a=createNode("ELSE",0,yylineno);return ELSE;}
{WHILE} {printf("(%s,WHILE)\n",yytext);yylval.a=createNode("WHILE",0,yylineno);return WHILE;}
{PLUS} {printf("(%s,PLUS)\n",yytext);yylval.a=createNode("PLUS",0,yylineno);return PLUS;}
{MINUS} {printf("(%s,MINUS)\n",yytext);yylval.a=createNode("MINUS",0,yylineno);return MINUS;}
{MUL} {printf("(%s,MUL)\n",yytext);yylval.a=createNode("MUL",0,yylineno);return MUL;}
{DIV} {printf("(%s,DIV)\n",yytext);yylval.a=createNode("DIV",0,yylineno);return DIV;}
{INTEGER} {printf("(%s,INTEGER)\n",yytext);yylval.a=createNode("INTEGER",0,yylineno);return INTEGER;}
{FLOAT} {printf("(%s,FLOAT)\n",yytext);yylval.a=createNode("FLOAT",0,yylineno);return FLOAT;}
{CHAR} {printf("(%s,CHAR)\n",yytext);yylval.a=createNode("CHAR",0,yylineno);return CHAR;}
{ID} {printf("(%s,INDENTIFIER)\n",yytext);yylval.a=createNode("ID",0,yylineno);return ID;}
{SEMI} {printf("(%s,SEMI)\n",yytext);yylval.a=createNode("SEMI",0,yylineno);return SEMI;}
{COMMA} {printf("(%s,COMMA)\n",yytext);yylval.a=createNode("COMMA",0,yylineno);return COMMA;}
{SPDOT} {printf("(%s,SPDOT)\n",yytext);yylval.a=createNode("SPDOT",0,yylineno);return SPDOT;}
{ASSIGNOP} {printf("(%s,ASSIGNOP)\n",yytext);yylval.a=createNode("ASSIGNOP",0,yylineno);return ASSIGNOP;}
{GRAET} {printf("(%s,GRAET)\n",yytext);yylval.a=createNode("GRAET",0,yylineno);return GRAET;}
{LESS} {printf("(%s,LESS)\n",yytext);yylval.a=createNode("LESS",0,yylineno);return LESS;}
{GASSIGN} {printf("(%s,GASSIGN)\n",yytext);yylval.a=createNode("GASSIGN",0,yylineno);return GASSIGN;}
{LESSASSIGN}  {printf("(%s,LESSASSIGN)\n",yytext);yylval.a=createNode("LESSASSIGN",0,yylineno);return LESSASSIGN;}
{PLUSASSIGN}  {printf("(%s,PLUSASSIGN)\n",yytext);yylval.a=createNode("PLUSASSIGN",0,yylineno);return PLUSASSIGN;}
{SUBASSIGN} {printf("(%s,SUBASSIGN)\n",yytext);yylval.a=createNode("SUBASSIGN",0,yylineno);return SUBASSIGN;}
{MULASSIGN} {printf("(%s,MULASSIGN)\n",yytext);yylval.a=createNode("MULASSIGN",0,yylineno);return MULASSIGN;}
{DIVASSIGN} {printf("(%s,DIVASSIGN)\n",yytext);yylval.a=createNode("DIVASSIGN",0,yylineno);return DIVASSIGN;}
{DOUBLEASSIGN} {printf("(%s,DOUBLEASSIGN)\n",yytext);yylval.a=createNode("DOUBLEASSIGN",0,yylineno);return DOUBLEASSIGN;}
{DOUBLEPLUS} {printf("(%s,DOUBLEPLUS)\n",yytext);yylval.a=createNode("DOUBLEPLUS",0,yylineno);return DOUBLEPLUS;}
{DOUBLESUB} {printf("(%s,DOUBLESUB)\n",yytext);yylval.a=createNode("DOUBLESUB",0,yylineno);return DOUBLESUB;}
{NOTASSIGN} {printf("(%s,NOTASSIGN)\n",yytext);yylval.a=createNode("NOTASSIGN",0,yylineno);return NOTASSIGN;}
{LP} {printf("(%s,LP)\n",yytext);yylval.a=createNode("LP",0,yylineno);return LP;}
{RP} {printf("(%s,RP)\n",yytext);yylval.a=createNode("RP",0,yylineno);return RP;}
{LB} {printf("(%s,LB)\n",yytext);yylval.a=createNode("LB",0,yylineno);return LB;}
{RB} {printf("(%s,RB)\n",yytext);yylval.a=createNode("RB",0,yylineno);return RB;}
{LC} {printf("(%s,LC)\n",yytext);yylval.a=createNode("LC",0,yylineno);return LC;}
{RC} {printf("(%s,RC)\n",yytext);yylval.a=createNode("RC",0,yylineno);return RC;}
{EOL} {yycolumn = 1; }
{SINGLEQUOTE} {}
{COMMENT} {}
{AERROR} { printf("Error type A at line %d: Mystirious charachter '%s'\n",yylineno,yytext);}
%%
int yywrap()
{
    return 1;
}