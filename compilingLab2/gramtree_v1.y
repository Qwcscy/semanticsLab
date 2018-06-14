%{
	#include<stdio.h>
    #include<unistd.h>
    #include"lex.yy.c"
    #include<iostream>
    using namespace std;
%}
%error-verbose
%locations

%union{
gramTree a;
}
%token <a> INTEGER FLOAT CHAR 
%token <a> TYPE RETURN IF ELSE WHILE ID SPACE SEMI COMMA SPDOT ASSIGNOP GREAT LESS GASSIGN
%token <a> LESSASSIGN DOUBLEASSIGN NOTASSIGN PLUSASSIGN SUBASSIGN DIVASSIGN MULASSIGN DOUBLEPLUS DOUBLESUB
%token <a> PLUS MINUS MUL DIV LP RP LB RB LC RC AERROR STRUCT
%type  <a> Program ExtDefList ExtDef ExtDecList Specifire VarDec  FunDec VarList ParamDec Compst StmtList Stmt DefList Def DecList Dec Exp Args StructSpecifire OptTag Tag

%right ASSIGNOP
%left GRAET LESS
%left GASSIGN LESSASSIGN DOUBLEASSIGN NOTASSIGN 
%left PLUS MINUS
%left DOUBLEPLUS DOUBLESUB 
%left MUL DIV 
%left PLUSASSIGN SUBASSIGN DIVASSIGN MULASSIGN 
%left LP RP LB RB SPDOT 
%%
Program:{$$=NULL;}|ExtDefList
 {
    $$=createNode("Program",1,$1);
    printf("\nprint syntax tree:\n");
    traTree($$,0);
    printf("syntax tree has finished printing !\n\n");
    startCreateSymbolTable($1);
    printf("semantic analysis has finished !\n");
}
    ;
ExtDefList:ExtDef ExtDefList {$$=createNode("ExtDefList",2,$1,$2);}
	|{$$=NULL;}
	;
ExtDef:Specifire ExtDecList SEMI {$$=createNode("ExtVarDef",2,$1,$2);$$->type=$1->nameValue;}
	|Specifire SEMI	{$$=createNode("Struct",1,$1);}
	|Specifire FunDec Compst{
	$$=createNode("FuncDef",3,$1,$2,$3);$$->nameValue=$2->nameValue;$$->type=$1->nameValue;}
	;
ExtDecList:VarDec {$$=createNode("ExtDecList",1,$1);}
	|VarDec COMMA ExtDecList{$$=createNode("ExtDecList",2,$1,$3);
    }
	;
Specifire:TYPE {$$=createNode("Specifire",1,$1);$$->nameValue=$1->nameValue;}
|StructSpecifire {$$=$1;}
	;
StructSpecifire:STRUCT OptTag LC DefList RC//结构体定义，检查是否有重定义
{
    $$=createNode("StructDef",2,$2,$4);
    $$->nameValue=$2->nameValue;
}
	|STRUCT Tag{$$=createNode("StructRef",1,$2);$$->nameValue=$2->nameValue;}
	;
OptTag:ID {$$=createNode("OptTag",1,$1);$$->nameValue=$1->nameValue;}
	|{$$=createNode("OptTag",0,-1);}
	;
Tag:ID{$$=createNode("Tag",1,$1);$$->nameValue=$1->nameValue;}
	;
VarDec:ID {$$=createNode("VarID",1,$1);$$->nameValue=$1->nameValue;}
	|VarDec LB INTEGER RB {$$=createNode("ArrayDef",2,$1,$3);$$->nameValue=$1->nameValue;}//数组暂时不考虑
	;
FunDec:ID LP VarList RP {$$=createNode("FunDec",2,$1,$3);$$->nameValue=$1->nameValue;}
	|ID LP RP {$$=createNode("FunDec",1,$1);$$->nameValue=$1->nameValue;}
	;
VarList:ParamDec COMMA VarList {$$=createNode("ParamList",2,$1,$3);}
	|ParamDec {$$=createNode("ParamList",1,$1);}
	;
ParamDec:Specifire VarDec{$$->type=$1->nameValue;$$->nameValue=$2->nameValue;}
    ;
/*Statement*/
Compst:LC DefList StmtList RC {$$=createNode("Compst",2,$2,$3);}
	;
StmtList:Stmt StmtList{$$=createNode("StmtList",2,$1,$2);}
	|{$$=NULL;}
	;
Stmt:Exp SEMI {$$=createNode("ExpStmt",1,$1);}
	|Compst {$$=$1;}
	|RETURN Exp SEMI {$$=createNode("RETURNStmt",1,$2);}
	|IF LP Exp RP Stmt {$$=createNode("IFStmt",2,$3,$5);}
	|IF LP Exp RP Stmt ELSE Stmt {$$=createNode("IFStmt",3,$3,$5,$7);}
	|WHILE LP Exp RP Stmt {$$=createNode("WHILEStmt",2,$3,$5);}
	;
DefList:Def DefList{$$=createNode("DefList",2,$1,$2);}
	| {$$=NULL;}
	;
Def:Specifire DecList SEMI {$$=createNode("varDef",2,$1,$2);$$->type=$1->nameValue;}
	;
DecList:Dec {$$=createNode("DecList",1,$1);}
	|Dec COMMA DecList {$$=createNode("DecList",2,$1,$3);}
	;
Dec:VarDec {$$=$1;}
	|VarDec ASSIGNOP Exp {$$=createNode("ASSIGNOP",2,$1,$3);$$->nameValue="ASSIGNOP";}
	;
/*Expressions*/
Exp:Exp ASSIGNOP Exp{$$=createNode("ASSIGNOP",2,$1,$3);$$->nameValue="ASSIGNOP";}
        |Exp PLUS Exp{$$=createNode("PLUS",2,$1,$3);$$->nameValue="PLUS";}
        |Exp MINUS Exp{$$=createNode("MINUS",2,$1,$3);$$->nameValue="MINUS";}
        |Exp MUL Exp{$$=createNode("MUL",2,$1,$3);$$->nameValue="MUL";}
        |Exp DIV Exp{$$=createNode("DIV",2,$1,$3);$$->nameValue="DIV";}
        |Exp GREAT Exp{$$=createNode("GREAT",2,$1,$3);$$->nameValue="GREAT";}
        |Exp LESS Exp{$$=createNode("LESS",2,$1,$3);$$->nameValue="LESS";}
        |Exp GASSIGN Exp{$$=createNode("GASSIGN",2,$1,$3);$$->nameValue="GASSIGN";}
        |Exp LESSASSIGN Exp{$$=createNode("LESASSIGN",2,$1,$3);$$->nameValue="LESSASSIGN";}
        |Exp DOUBLEASSIGN Exp{$$=createNode("DOUBLEASSIGN",2,$1,$3);$$->nameValue="DOUBLEASSIGN";}
        |Exp NOTASSIGN Exp{$$=createNode("NOTASSIGN",2,$1,$3);$$->nameValue="NOTASSIGN";}
        |Exp PLUSASSIGN Exp{$$=createNode("PLUSASSIGN",2,$1,$2,$3);$$->nameValue="PULSASSIGN";}
        |Exp SUBASSIGN Exp{$$=createNode("SUBASSIGN",2,$1,$3);$$->nameValue="SUBASSIGN";}
        |Exp MULASSIGN Exp{$$=createNode("MULASSIGN",2,$1,$3);$$->nameValue="MULASSIGN";}
        |Exp DIVASSIGN Exp{$$=createNode("DIVASSIGN",2,$1,$3);$$->nameValue="DIVASSIGN";}
        |Exp DOUBLEPLUS {$$=createNode("DOUBLEPLUS",1,$1);$$->nameValue="DOUBLEPLUS";}
        |Exp DOUBLESUB {$$=createNode("DOUBLESUB",1,$1);$$->nameValue="DOUBLESUB";}
        |DOUBLEPLUS Exp{$$=createNode("DOUBLEPLUS",1,$2);$$->nameValue="DOUBLEPLUS";}
        |DOUBLESUB Exp{$$=createNode("DOUBLESUB",1,$2);$$->nameValue="DOUBLESUB";}
        |LP Exp RP{$$=$2;}
        |MINUS Exp {$$=createNode("MINUSSIGN",1,$2);$$->nameValue="MINUSSIGN";}
        |ID LP Args RP {$$=createNode("FUNCALL",2,$1,$3);$$->nameValue=$1->nameValue;}
        |ID LP RP {$$=createNode("FUNCALL",1,$1);$$->nameValue=$1->nameValue;}
        |Exp LB Exp RB {$$=createNode("ARRAY",2,$1,$3);}
        |Exp SPDOT ID {$$=createNode("STRUCTREF",2,$1,$3);$$->nameValue=$3->nameValue;}
        |ID {$$->nameValue=$1->nameValue;}
        |INTEGER {$$=$1;$$->type="int";}
        |FLOAT {$$=$1;$$->type="float";}
        |CHAR {$$=$1;$$->type="char";}
        ;
Args:Exp COMMA Args {$$=createNode("Args",2,$1,$3);}
        |Exp {$$=createNode("Args",1,$1);}
        ;
%%
#include<stdarg.h>
void yyerror(const char*s,...) //变长参数错误处理函数
{
    va_list ap;
    va_start(ap,s);
    //(stderr,"error at line %d:",yylineno);//错误行号
    fprintf(stderr, "Error at Line %d Column %d: ", yylineno, yylloc.first_column);
    vfprintf(stderr,s,ap);
    fprintf(stderr,"\n");
} 