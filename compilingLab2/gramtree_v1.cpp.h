/*************************************************************************
    > File Name: gramtree_v1.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年05月15日 星期二 20时04分40秒
 ************************************************************************/
extern int yylineno;//行号
extern char* yytext;//词语
extern FILE* yyin;
extern int yyparse();
void yyerror(const char *s,...);//错误处理函数
/*抽象语法树节点*/
#include <string>
#include<vector>
#include <map>
#include <iostream>
using namespace std;
struct gram
{
    int line;//行号
    vector<struct gram*> child;
    string name;
    union//共用体用来存放ID/TYPE/INTEGER/FLOAT结点的值
    {
        string nameValue;             //由标识符生成的叶结点
        int intValue;                 //由整常数生成的叶结点
        int floatValue;               //由浮点常数生成的叶结点
        char charValue;
    };
    string funcName;//用来标识参数列表结点对应的函数名
    string type;//参数的类型，或者函数的类型，变量的类型
};
class param{
    string name;
    string type;
public:
    param(){};
    param(string m_name,string m_type):name(m_name),type(m_type){};
public:
    string getName(){return name;}
    string getType(){return type;}
};

class symbolTable {
public:
    static int ifStmtNum;
    string kind;
    string type;
    map<string,symbolTable*> *childNode;
    vector<param *> args;//函数参数的类型
    symbolTable *parentNode;//当前结点的父亲结点，根节点为空
    symbolTable(map<string,symbolTable*> *m_childNode, symbolTable *m_p) : childNode(m_childNode), parentNode(m_p) {}
    symbolTable(string m_kind, string m_type, map<string,symbolTable*> *m_childNode, symbolTable *m_p) :
            kind(m_kind), type(m_type),childNode(m_childNode), parentNode(m_p) {};
    bool setArgs(string name,string type){
        for(auto i:args)
            if (i->getName()==name)
                return false;
        args.push_back(new param(name,type));
        return true;
    }
};
typedef struct gram gramNode;
typedef gramNode* gramTree;
typedef map<string,symbolTable*> node;
 /*构造抽象语法树，变长参数，name:语法单元名字；num:变长参数中语法结点的个数*/
gramTree createNode(string name,int num,...);
    /*遍历抽象语法树，level为树的层数*/
void traTree(gramTree tree,int level);
void startCreateSymbolTable(gramTree T);
void createSymbolTable(gramTree T,node* curNode,symbolTable* parentNode);
