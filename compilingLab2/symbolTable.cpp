#include <iostream>
#include <map>
#include <vector>
#include "gramtree_v1.cpp.h"
using namespace std;
int symbolTable::ifStmtNum=0;
symbolTable* root=new symbolTable(nullptr, nullptr);
bool insertVarNode(string kind,string type,string name,node* curNode,symbolTable* parentNode)//插入变量结点
//curNode指向一个map
{
    if(curNode->count(name)==1)//变量重复定义
        return false;
    else{
        symbolTable* newNode=new symbolTable(kind,type,nullptr,parentNode);
        (*curNode)[name]=newNode;
    }
}
//在符号表中查找idName是否定义，
//是则并返回其类型idType,否则返回空
//如果在当前层找不到，需要向上搜索
string findId(string idName,node* curNode,symbolTable* parentNode)
{
    if(parentNode)
    {
        if(curNode->count(idName)==1)
            return curNode->at(idName)->type;
        else{
            parentNode=parentNode->parentNode;
            curNode=parentNode->childNode;
            findId(idName,curNode,parentNode);//向上搜索
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
    cout<<"differnt type between "<<firstType<<"and "<<secndType<<" can't be operation"<<endl;
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
                if(T->child[1]->name=="ID")//只有一个变量
                {
                    if(!insertVarNode("VAR",T->type,T->child[1]->nameValue,curNode,parentNode))//变量重名
                        error(T->line,"redeclaration of",T->child[1]->nameValue);
                    return;
                }
                else{//定义了多个外部变量
                    createSymbolTable(T->child[1],curNode,parentNode);
                    return;
                }
            }
            else if(T->name=="ExtDecList")//处理多个外部变量
            {
                gramTree T0=T;
                while(T0->name=="ExtDecList")
                {
                    if(!insertVarNode("VAR",T0->type,T0->child[1]->nameValue,curNode,parentNode))//变量重名
                        error(T0->line,"redeclaration of",T0->child[1]->nameValue);
                    T0->child[1]->type=T0->type;//变量类型向下传递
                    T0=T0->child[1];
                }
                /*最后一个变量*/
                if(!insertVarNode("VAR",T0->type,T0->child[1]->nameValue,curNode,parentNode))//变量重名
                    error(T0->line,"variable redeclaration of",T0->child[1]->nameValue);
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
                    error(T->line,"struct redeclaration of",T->child[0]->nameValue);
                //下面获取结构体各个成员的类型和id
                symbolTable* newNode=new symbolTable("STRUCT",T->child[0]->nameValue, nullptr,parentNode);
                newNode->childNode=new map<string,symbolTable*>;//生成新的作用域
                (*curNode)[T->child[0]->nameValue]=newNode;
                curNode=curNode->at(T->child[0]->nameValue)->childNode;
                parentNode=curNode->at(T->child[0]->nameValue);//结构体结点成为新的父亲结点
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
            else if(T->name=="FuncDef")//函数定义，注意进入新的定义域
            {
                createSymbolTable(T->child[1],curNode,parentNode);//处理函数名和参数列表
                curNode=curNode->at(T->nameValue)->childNode;//进入新的作用域
                parentNode=curNode->at(T->nameValue);
                T->child[2]->funcName=T->nameValue;//标识该函数体结点所属的函数结点
                createSymbolTable(T->child[2],curNode,parentNode);//处理函数体
                return ;
            }
            else if(T->name=="FunDec")
            {
                if(!insertVarNode("FUNCTION",T->type,T->nameValue,curNode,parentNode))//新建函数结点
                    error(T->line,"function redeclaration of",T->nameValue);
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
                    error(T->line,"undfined function of ",T->nameValue);
                if(T->child.size()>1)//有参数，检查参数类型
                {
                    gramTree T0=T->child[0];
                    vector<string> varType;
                    while(T0->name=="Args")
                    {
                        varType.push_back(T0->child[0]->type);
                        T0=T->child[1];
                    }
                    varType.push_back(T0->nameValue);//最后一个参数
                    if(root->childNode->at(T->nameValue)->args.size()>varType.size())
                        error(T->line,"too few parameters","");
                    if(root->childNode->at(T->nameValue)->args.size()<varType.size())
                        error(T->line,"too many parameters","");
                    for(int i=0;i<root->childNode->at(T->nameValue)->args.size();i++)
                    {
                        if(root->childNode->at(T->nameValue)->args[i]->getType()!=varType[i])
                        {
                            error(T->line,"parameter type errors","");
                            break;
                        }
                    }
                }
                return;
            }
            else if(T->name=="ParamList")
            {
                if(!curNode->at(T->funcName)->setArgs(T->type,T->nameValue))//插入参数，参数重复定义
                    error(T->line,"redeclaration parameter of ",T->nameValue);
                if(T->child.size()>1)//多个参数
                    createSymbolTable(T->child[1],curNode,parentNode);//处理其他参数
                return ;
            }
            else if(T->name=="Compst")//函数体，要进入新的作用域，完成后又要退出作用域
            {
                curNode=curNode->at(T->funcName)->childNode;
                parentNode=curNode->at(T->funcName);//进新作用域
                createSymbolTable(T->child[0],curNode,parentNode);
                createSymbolTable(T->child[1],curNode,parentNode);
                parentNode=parentNode->parentNode;//出作用域
                curNode=parentNode->childNode;
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
                if(T->child[0]->type!=parentNode->type)//返回值类型和函数表达式比较
                    cout<<"line "<<T->line<<" error:return makes type "<<
                        T->child[0]->type<<"from type "<<parentNode->type<<" without a cast"<<endl;
                return ;
            }
            else if(T->name=="IFStmt"||T->name=="WHILEStmt")//条件语句if、if_else和while
            {
                symbolTable::ifStmtNum++;
                createSymbolTable(T->child[0],curNode,parentNode);//处理条件表达式
                if(T->child[0]->type!="int"||T->child[0]->type!="float")//类型不对
                    error(T->line,"条件表达式类型错误","");
                string name=to_string( symbolTable::ifStmtNum++)+T->name;//随机生成块名
                symbolTable* newNode=new symbolTable(nullptr, nullptr);
                (*curNode)[name]=newNode;
                curNode=curNode->at(name)->childNode;
                parentNode=curNode->at(name);//进入新的作用域
                createSymbolTable(T->child[1],curNode,parentNode);//处理if内的语句
                parentNode=parentNode->parentNode;//出作用域
                curNode=parentNode->childNode;

                if(T->child.size()>2)//有else
                {
                    symbolTable::ifStmtNum++;
                    name=to_string( symbolTable::ifStmtNum)+T->name;//随机生成块名
                    symbolTable* newNode=new symbolTable(nullptr, nullptr);
                    (*curNode)[name]=newNode;
                    curNode=curNode->at(name)->childNode;
                    parentNode=curNode->at(name);//进入新的作用域
                    createSymbolTable(T->child[2],curNode,parentNode);//处理else内的语句
                    parentNode=parentNode->parentNode;//出作用域
                    curNode=parentNode->childNode;
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
                T->child[1]->type=T->child[0]->nameValue;//类型向下传递
                gramTree T0=T->child[1];
                while(T0->name=="DecList")
                {
                    T0->child[0]->type=T0->type;
                    T0->child[1]->type=T0->type;
                    if(T0->child[0]->name=="VarID")//只有一个变量
                    {
                        if(!insertVarNode("VAR",T0->type,T0->child[0]->nameValue,curNode,parentNode))//变量重名
                            error(T0->line,"redeclaration of",T0->child[0]->nameValue);
                    }
                    else if(T0->child[0]->name=="ASSIGNOP")//如果有变量的初始化
                    {
                        if(!insertVarNode("VAR",T0->type,T0->child[0]->child[0]->nameValue,curNode,parentNode))//变量重名
                            error(T0->line,"redeclaration of",T0->child[0]->child[0]->nameValue);
                        createSymbolTable(T0->child[1],curNode,parentNode);//处理赋值语句,判断类型是否匹配，如果右边有标识符，
                        // 还要判断该标识符是否定义
                        if(T0->type!=T0->child[1]->type)//左右两边类型不匹配
                            typeError(T0->line,T0->type,T0->child[1]->type);
                    }
                }
                return;
            }
            else if(T->name=="ASSIGNOP")//为一个等式
            {
                if(T->child[0]->name!="ID")
                    error(T->line," ","赋值语句需要左值");
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
                    //T->type=T->child[0]->type;
                    T->type="int";//条件表达式自动转化为int
                else opTypeError(T->line,T->child[0]->type,T->child[1]->type);
                return ;
            }
            else if(T->name=="DOUBLEPLUS"||T->name=="DOUBLESUB")//++和--
            {
                createSymbolTable(T->child[0],curNode,parentNode);
                if(T->child[0]->type!="int")
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
            }
            else if(T->name=="INTEGER"||T->name=="FLOAT"||T->name=="CHAR")
                return;
        }
    }
void startCreateSymbolTable(gramTree T)
{
    node* curNode=new map<string,symbolTable*>();
    root->childNode=curNode;
    symbolTable* parentNode=root;
    createSymbolTable(T,curNode,parentNode);
}

