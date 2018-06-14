#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "gramtree_v1.cpp.h"
#include <string.h>
#include <string>
#include<iostream>

using namespace std;
int symbolTable::ifStmtNum=0;
symbolTable* root=new symbolTable(nullptr, nullptr);
typedef map<string,symbolTable*> node;
bool insertVarNode(string kind,string type,string name,node* curNode,symbolTable* parentNode)//插入变量结点
//curNode指向一个map
{
    if(curNode->count(name)==1)//变量重复定义
        return false;
    else{
        symbolTable* newNode=new symbolTable(kind,type,new map<string,symbolTable*>(),parentNode);
        (*curNode)[name]=newNode;
        return true;
    }
}
//在符号表中查找idName是否定义，
//是则并返回其类型idType,否则返回空
//如果在当前层找不到，需要向上搜索
string findId(string idName,node* curNode,symbolTable* parentNode)
{
    if(parentNode&&curNode)
    {
        if(curNode->count(idName)==1)
            return curNode->at(idName)->type;
        else{
            parentNode=parentNode->parentNode;
            if(parentNode){
                curNode=parentNode->childNode;
                string type=findId(idName,curNode,parentNode);//向上搜索
                return type;
            }
            else return "";
        }
    }
    return "";
}
void error(int line,string msg1,string msg2)//报错信息函数
{
    cout<<"line "<<line<<": error: "<<msg1<<"  "<<msg2<<endl;
}
void typeError(int line,string typeTarget,string typeSorce)
{
    cout<<"line "<<line<<": error: ";
    cout<<"incompatible types when initializing type ‘"<<typeTarget<<"’ using type ‘"<<typeSorce<<"'"<<endl;
}
void opTypeError(int line,string firstType,string secndType)
{
    cout<<"line "<<line<<": error: ";
    cout<<"differnt type between "<<firstType<<" and "<<secndType<<" can't be operation"<<endl;
}
/*开始建立符号表并进行语意义检查*/
void createSymbolTable(gramTree T,node* curNode,symbolTable* parentNode)
{
    if(T)
    {
        if(T->name=="ExtDefList")
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            return;
        }
        else if(T->name=="ExtVarDef")//外部变量定义
        {
            T->child[1]->type=T->type;
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            return;
        }
        else if(T->name=="ExtDecList")//处理多个外部变量
        {
            //先处理第一个变量
            createSymbolTable(T->child[0],curNode,parentNode);
            if(!insertVarNode("VAR",T->type,T->child[0]->nameValue,curNode,parentNode))
                error(T->line,"redeclaration of",T->child[0]->nameValue);
            if(T->child.size()>1)//有多个变量的时候
            {
                T->child[1]->type=T->type;//变量类型向下传递
                T=T->child[1];
                createSymbolTable(T->child[1],curNode,parentNode);
            }
            return ;
        }
        else if(T->name=="Struct")//结构体
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            return ;
        }
        else if(T->name=="StructDef")//结构体定义，此处要进入新的作用域
        {
            if(curNode->count(T->child[0]->nameValue)==1)//重复定义
            {
                error(T->line,"struct redeclaration of",T->child[0]->nameValue);
                return ;
            }
            //下面获取结构体各个成员的类型和id
            symbolTable* newNode=new symbolTable("STRUCT",T->child[0]->nameValue,new map<string,symbolTable*>(),parentNode);
            newNode->childNode=new map<string,symbolTable*>;//生成新的作用域
            (*curNode)[T->child[0]->nameValue]=newNode;
            parentNode=curNode->at(T->child[0]->nameValue);//结构体结点成为新的父亲结点
            curNode=curNode->at(T->child[0]->nameValue)->childNode;
            createSymbolTable(T->child[1],curNode,parentNode);
            parentNode=parentNode->parentNode;//出作用域
            curNode=parentNode->childNode;
            return;
        }
        else if(T->name=="StructRef")//结构体引用，检查是否定义
        {
            if(root->childNode->count(T->child[0]->nameValue)==0)//在顶层查找结构体
                error(T->line,"undifined struct of ",T->child[0]->nameValue);//未定义结构体
            return;
        }
            //结构体变量引用成员，比如a.b，先查看a是否定义，再查看结构体a内有没有成员b
        else if(T->name=="STRUCTREF")
        {
            string type=findId(T->child[0]->nameValue,curNode,parentNode);
            //type返回值为结构体类型
            if(type.empty())
                error(T->line,"undifined struct variable of ",T->child[0]->nameValue);//未定义结构体变量
            if(root->childNode->count(type)==0)
                error(T->line,"undefined struct of ",type);
            //检查该成员是否定义
            string memType=findId(T->nameValue,root->childNode->at(type)->childNode,root->childNode->at(type));
            if(!memType.empty())//没有该成员
                T->type=memType;
            else cout<<"line "<<T->line<<": error 'struct "<<type<<"' has no member named '"<<T->nameValue<<"'"<<endl;
            return;
        }
        else if(T->name=="FuncDef")//函数定义，注意进入新的定义域
        {
            if(!insertVarNode("FUNCTION",T->type,T->nameValue,curNode,parentNode))//新建函数结点
            {
                error(T->line,"function redeclaration of",T->nameValue);
                return ;
            }
            createSymbolTable(T->child[0],curNode,parentNode);
            T->child[1]->type=T->type;//函数类型向下传递
            createSymbolTable(T->child[1],curNode,parentNode);//处理函数名和参数列表
            T->child[2]->funcName=T->nameValue;//标识该函数体结点所属的函数结点c
            parentNode=curNode->at(T->nameValue);//进入新的作用域
            curNode=curNode->at(T->nameValue)->childNode;
            createSymbolTable(T->child[2],curNode,parentNode);//处理函数体
            parentNode=parentNode->parentNode;//出作用域
            curNode=parentNode->childNode;
            return ;
        }
        else if(T->name=="FunDec")
        {
            if(T->child.size()>1)//有参数
            {
                T->child[1]->funcName=T->nameValue;//将列表结点标识为该函数的列表结点
                createSymbolTable(T->child[1],curNode,parentNode);//处理函数参数列表结点
            }
            return;
        }
        else if(T->name=="FUNCALL")
        {
            if(root->childNode->count(T->nameValue)==0)//函数未定义
            {
                error(T->line,"undfined function of ",T->nameValue);
                return ;
            }
            if(T->child.size()>1)//有参数，检查参数类型
            {
                gramTree T0=T->child[1];
                vector<string> varType;
                while(T0->name=="Args")
                {
                    createSymbolTable(T0->child[0],curNode,parentNode);
                    varType.push_back(T0->child[0]->type);
                    if(T0->child.size()>1)
                        T0=T0->child[1];
                    else break;
                }
                if(root->childNode->at(T->nameValue)->args.size()>varType.size())
                    error(T->line,"too few parameters","");
                if(root->childNode->at(T->nameValue)->args.size()<varType.size())
                    error(T->line,"too many parameters","");
                for(int i=0;i<root->childNode->at(T->nameValue)->args.size();i++)
                {
                    if(root->childNode->at(T->nameValue)->args[i]->getType()!=varType[i])
                    {
                        cout<<"line "<<T->line<<": error: expected '"<<root->childNode->at(T->nameValue)->args[i]->getType()<<
                        "' but argument is of type '"<<varType[i]<<"'"<<endl;
                        break;
                    }
                }
            }
            return;
        }
        else if(T->name=="ParamList")
        {
            if(!curNode->at(T->funcName)->setArgs(T->child[0]->nameValue,T->child[0]->type))//插入参数，参数重复定义
                error(T->line,"redeclaration parameter of ",T->child[0]->nameValue);
            if(T->child.size()>1)//多个参数
            {
                T->child[1]->funcName=T->funcName;
                createSymbolTable(T->child[1],curNode,parentNode);//处理其他参数
            }
            return ;
        }
        else if(T->name=="Compst")//函数体，要进入新的作用域，完成后又要退出作用域
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            return;
        }
        else if(T->name=="StmtList")
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            return ;
        }
        else if(T->name=="ExpStmt")
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            return ;
        }
        else if(T->name=="RETURNStmt")
        {
            createSymbolTable(T->child[0],curNode,parentNode);//处理返回值表达式
            if(T->child[0]->type!=parentNode->type)//返回值类型和函数返回值比较
            {
                cout<<"line "<<T->line<<": error: return makes type '"<<
                    T->child[0]->type<<"' from type '"<<parentNode->type<<"' without a cast"<<endl;
            }
            return ;
        }
        else if(T->name=="IFStmt"||T->name=="WHILEStmt")//条件语句if、if_else和while
        {
            symbolTable::ifStmtNum++;
            createSymbolTable(T->child[0],curNode,parentNode);//处理条件表达式
            if(T->child[0]->type!="int")
                cout<<"line "<<T->line<<": error: used "<<T->child[0]->type<<" type value where int is required"<<endl;
            string name=to_string(symbolTable::ifStmtNum++)+T->name;//随机生成块名
            symbolTable* newNode=new symbolTable(new map<string,symbolTable*>(),parentNode);
            (*curNode)[name]=newNode;
            parentNode=curNode->at(name);//进入新的作用域
            curNode=curNode->at(name)->childNode;
            createSymbolTable(T->child[1],curNode,parentNode);//处理if内的语句
            parentNode=parentNode->parentNode;//出作用域
            curNode=parentNode->childNode;
            if(T->child.size()>2)//有else
            {
                symbolTable::ifStmtNum++;
                name=to_string( symbolTable::ifStmtNum)+T->name;//随机生成块名
                symbolTable* newNode=new symbolTable(new map<string,symbolTable*>(),parentNode);
                (*curNode)[name]=newNode;
                parentNode = curNode->at(name);//进入新的作用域
                curNode = curNode->at(name)->childNode;
                createSymbolTable(T->child[2], curNode, parentNode);//处理else内的语句
                parentNode = parentNode->parentNode;//出作用域
                curNode = parentNode->childNode;
            }
            return;
        }
        else if(T->name=="DefList")
        {
            createSymbolTable(T->child[0],curNode,parentNode);//处理第一条局部变量的声明
            createSymbolTable(T->child[1],curNode,parentNode);//处理剩余语句的声明
            return;
        }
        else if(T->name=="varDef")//处理第一个局部变量的定义
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            T->child[1]->type=T->child[0]->nameValue;//类型向下传递
            gramTree T0=T->child[1];
            while(T0->name=="DecList")
            {
                if(T0->child[0]->name=="VarID")//只有一个变量
                {
                    if(!insertVarNode("VAR",T0->type,T0->child[0]->nameValue,curNode,parentNode))//变量重名
                        error(T0->line,"redeclaration variable of",T0->child[0]->nameValue);
                }
                else if(T0->child[0]->name=="ASSIGNOP")//如果有变量的初始化
                {
                    T0->child[0]->type=T0->type;//类型传递
                    if(!insertVarNode("VAR",T0->type,T0->child[0]->child[0]->nameValue,curNode,parentNode))//变量重名
                        error(T0->line,"redeclaration variable of",T0->child[0]->child[0]->nameValue);
                    createSymbolTable(T0->child[0]->child[1],curNode,parentNode);//处理赋值语句,判断类型是否匹配，如果右边有标识符，
                    // 还要判断该标识符是否定义
                    if(T0->type!=T0->child[0]->child[1]->type)//左右两边类型不匹配
                        typeError(T0->line,T0->type,T0->child[0]->child[1]->type);
                }
                if(T0->child.size()==1)
                    break;
                else
                {
                    T0->child[1]->type=T0->type;
                    T0=T0->child[1];
                }//继续处理剩下的变量
            }
            return;
        }
        else if(T->name=="ASSIGNOP")//为一个等式
        {
            if(T->child[0]->name!="ID"&&T->child[0]->name!="STRUCTREF")
                error(T->line,"lvalue required as left operand of assignment","");
            else{
                createSymbolTable(T->child[0],curNode,parentNode);
                createSymbolTable(T->child[1],curNode,parentNode);
                if(T->child[0]->type!=T->child[1]->type)//左右两边类型不匹配
                    typeError(T->line,T->child[0]->type,T->child[1]->type);
                T->type=T->child[1]->type;
            }
            return;
        }
        else if(T->name=="SUB"||T->name=="MINUS"||T->name=="MUL"||T->name=="DIV"||T->name=="GREAT"||
                T->name=="LESS"||T->name=="GASSIGN"||T->name=="LESSASSIGN"||T->name=="SUBASSIGN"||
                T->name=="PLUSASSIGN"||T->name=="MULASSIGN"||T->name=="DIVASSIGN")
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            if(T->child[0]->type==T->child[1]->type&&(T->child[0]->type=="int"||T->child[0]->type=="float"))
                T->type=T->child[0]->type;
            else  opTypeError(T->line,T->child[0]->type,T->child[1]->type);
            return ;
        }
        else if(T->name=="NOTASSIGN"||T->name=="DOUBLEASSIGN")
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            createSymbolTable(T->child[1],curNode,parentNode);
            if(T->child[0]->type==T->child[1]->type)
                T->type="int";//条件表达式自动转化为int
            else opTypeError(T->line,T->child[0]->type,T->child[1]->type);
            return ;
        }
        else if(T->name=="DOUBLEPLUS"||T->name=="DOUBLESUB")//++和--
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            if(T->child[0]->type!="int"&&T->child[0]->type!="float")
                cout<<"line "<<T->line<<" error:invalid operation while type "<<
                    T->child[0]->type<<"using DOUBLEPLUS or DOUBLESUB"<<endl;
            else T->type=T->child[0]->type;
            return ;
        }
        else if(T->name=="MINUSSIGN")//负号
        {
            createSymbolTable(T->child[0],curNode,parentNode);
            if(T->child[0]->type!="int"||T->child[0]->type!="float")
                cout<<"line "<<T->line<<" error:invalid operation while type "<<
                    T->child[0]->type<<"using MINUSSIGN"<<endl;
            else T->type=T->child[0]->type;
            return ;
        }
        else if(T->name=="ID")//遇到变量的引用,查看是否定义
        {
            string type=findId(T->nameValue,curNode,parentNode);
            if(!type.empty())
                T->type=type;
            else error(T->line,"Undefined variable of ",T->nameValue);//id未定义
            return ;
        }
        else if(T->name=="INTEGER"||T->name=="FLOAT"||T->name=="CHAR")
            return;
    }
    return;
}
void printSymbolTable(node* curNode,symbolTable* parentNode,int level)
{
    if(curNode)
    {
        map<string,symbolTable*>::iterator iter;
        iter=curNode->begin();
        symbolTable* node= nullptr;
        while(iter!=curNode->end())
        {
            for(int i=0;i<level;i++)
                cout<<"     ";
            cout<<iter->first<<"\t\t";
            node=iter->second;
            cout<<node->kind<<"\t\t"<<node->type<<"\t\t";
            if(node->args.size()>0)
                for(auto i:node->args)
                    cout<<i->getType()<<"\t\t";
            string::size_type idx =iter->first.find("WHILEStmt");
            string::size_type idx1=iter->first.find("IFStmt");
            if(node->kind=="FUNCTION"||node->kind=="STRUCT"||idx!=string::npos||idx1!=string::npos)
            {
                cout<<endl;
                printSymbolTable(node->childNode,node,level+1);
            }
            iter++;
            cout<<endl;
        }
    }
}
void startCreateSymbolTable(gramTree T)
{
    node* curNode=new map<string,symbolTable*>();
    root->childNode=curNode;
    symbolTable* parentNode=root;
    createSymbolTable(T,curNode,parentNode);
    cout<<"nameValue\t\tkind\t\ttype\t\targs"<<endl;
    printSymbolTable(curNode,parentNode,0);
}


gramTree createNode(string name,int num,...)
{	
	int i;
	va_list valist;
	char* buf;
	gramTree current=(gramTree)malloc(sizeof(gramNode));
	current->name=name;
	gramTree tmp=(gramTree)malloc(sizeof(gramNode));
	va_start(valist,num);
	if(num>0)//为非终结符
	{
		tmp=va_arg(valist,gramTree);
        current->child.push_back(tmp);
        if(tmp)
            current->line=tmp->line;
        for(i=0;i<num-1;i++)
        {
            tmp=va_arg(valist,gramTree);
            current->child.push_back(tmp);
        }
	}
	else//为终结符
	{
		current->line=va_arg(valist,int);
		if(name=="ID"||name=="TYPE")//为标识符
		{
			buf=(char*)malloc(sizeof(strlen(yytext)));
			strcpy(buf,yytext);
            string ss(buf);
			current->nameValue=ss;
		}
		else if(name=="INTEGER"){//为int{
			current->intValue=atoi(yytext);
		}
		else if(name=="FLOAT"){
			current->floatValue=atof(yytext);
		}
		else if(name=="CHAR"){
			current->charValue=yytext[1];
		}
		else;
	}
	return current; 
}
void traTree(gramTree tree,int level)//遍历语法树，level为层数
{
	int i;
	if(tree)
	{
		for(i=0;i<level;i++)
			cout<<"  ";
		if(tree->line!=-1)
		{	
            cout<<tree->name<<":";
			if(tree->name=="ID"||tree->name=="TYPE")
                cout<<tree->nameValue;
			else if(tree->name=="INTEGER")
                cout<<tree->intValue;
			else if(tree->name=="FLOAT")
                cout<<tree->floatValue;
			else if(tree->name=="CHAR")
                cout<<tree->charValue;
			
			else
				cout<<"("<<tree->line<<")";
		}
        cout<<endl;
		for(int i=0;i<tree->child.size();i++)
            traTree(tree->child[i],level+1);
	}
}
int main(int argc, char** argv)
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }
    yyparse();
    return 0;
}

