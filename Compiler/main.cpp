#include <iostream>
#include <string>
#include <fstream>
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Symbol.h"
#include "TableItem.h"
#include "SymbolTable.h"
#include "Parser.h"
using namespace std;
#define DEBUG
string GetOpString(OpCode op)
{
    switch(op)
    {
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        case GIV: return "GIV";
        case STR: return "STR";
        case SETL: return "SETL";
        case FUNC: return "FUNC";
        case PARA: return "PARA";
        case PUSH: return "PUSH";
        case CALL: return "CALL";
        case JMP: return "JMP";
        case NEG: return "NEG";
        case RET: return "RET";
        case GET_RET: return "GET_RET";
        case GIV_ARR: return "GIV_ARR";
        case ARR_GIV: return "ARR_GIV";
        case BGR: return "BGR";
        case BEQ: return "BEQ";
        case BLS: return "BLS";
        case BNE: return "BNE";
        case BGE: return "BGE";
        case BLE: return "BLE";
        case WRITE: return "WRITE";
        case READ: return "READ";
        case END_FUNC: return "END_FUNC";
        case END_PROC: return "END_PROC";
        default: return "";
    }
}
int main()
{
    // 打印词法分析结果于文件LexerResult中
   /* ofstream lexOut, mdOut;
    lexOut.open("LexerResult.txt", ios::out);

 //   cout << "Please input source code file path:" << endl;
    string fileName("test.txt");
 //   cin >> fileName;
    ErrorHandler eh;
    Lexer lex(fileName, eh);
    vector<Symbol> words;
    while(true)
    {
        words.push_back(lex.GetSym());
        if((*words.rbegin()).type() == END)
            break;
    }
    if(eh.IsSuccessful())
    {
        cout<<"Build Succeeded! Now output the result......"<<endl;
        for(vector<Symbol>::iterator p = words.begin(); p != words.end(); p++)
        {
            lexOut<<"In line:"<<(*p).lineNum()<<"    "<<GetTypeName((*p).type())<<": "<<(*p).name()<<endl;
        }
        cout<<"Lexer Result in file: LexerResult.txt!"<<endl;
    }
    else
    {
        eh.PrintError();
    }*/

    ofstream lexOut, mdOut;
    lexOut.open("LexerResult.txt", ios::out);
    mdOut.open("MiddleCode.txt", ios::out);

 //   cout << "Please input source code file path:" << endl;
    string fileName("test.txt");
 //   cin >> fileName;
    ErrorHandler eh;
    Lexer lex(fileName, eh);
    MiddleCode mc;
    SymbolTable rt;
    Parser par(lex, eh, mc, rt);
    par.StartParsing();
    if(eh.IsSuccessful())
    {
        cout<<"Build Succeeded! Now output the result......"<<endl;
        for(vector<Symbol*>::iterator p = par.tokens.begin(); p != par.tokens.end(); p++)
        {
            lexOut<<"In line:"<<(*p)->lineNum()<<"    "<<GetTypeName((*p)->type())<<": "<<(*p)->name()<<endl;
        }
        cout<<"Lexer Result in file: LexerResult.txt!"<<endl;
        for(vector<midInstr>::iterator p = mc.code.begin(); p != mc.code.end(); p++)
        {
            string dsts, src1s, src2s;
            int src1n, src2n;
            if(p->dst)
                dsts = p->dst->name();
            mdOut.fill(' ');
            mdOut.setf(std::ios::left);
            mdOut.width(8);
            mdOut<<GetOpString(p->op);
            mdOut<<"    ";
            mdOut.width(5);
            mdOut<<dsts;
            mdOut<<"    ";
            mdOut.width(5);
            if(p->src1)
            {
                if(p->src1->type() == CONST_INT)
                    mdOut<<p->src1->value();
                else if(p->src1->type() == CONST_CHAR)
                    mdOut<<string(1,'\'') + string(1, char(p->src1->value())) + '\'';
                else
                    mdOut<<p->src1->name();
            }
            mdOut<<"    ";
            mdOut.width(5);
            if(p->src2)
            {
                if(p->src2->type() == CONST_INT)
                    mdOut<<p->src2->value();
                else if(p->src2->type() == CONST_CHAR)
                    mdOut<<string(1,'\'') + string(1, char(p->src2->value())) + '\'';
                else
                    mdOut<<p->src2->name();
            }
            mdOut<<endl;
        }
        cout<<"Middle Code in file: MiddleCode.txt!"<<endl;
    }
    else
    {
        eh.PrintError();
    }
    return 0;
}

/** 附加C0文法
＜加法运算符＞ ::= +｜-
＜乘法运算符＞  ::= *｜/
＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
＜字母＞   ::= _|a｜．．．｜z｜A｜．．．｜Z
＜数字＞   ::= ０｜＜非零数字＞
＜非零数字＞  ::= １｜．．．｜９
＜字符＞    ::=   '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                            | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
＜无符号整数＞  ::= ＜非零数字＞{＜数字＞}
＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
＜标识符＞    ::=  ＜字母＞｛＜字母＞｜＜数字＞｝
＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’ }
＜常量＞   ::=  ＜整数＞|＜字符＞
＜类型标识符＞      ::=  int | char
＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
＜参数＞    ::= ＜参数表＞
＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;
                      | ＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
＜循环语句＞   ::=  for‘(’＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞‘)’＜语句＞
＜步长＞    ::=  ＜非零数字＞｛＜数字＞｝
＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
＜情况子语句＞  ::=  case＜常量＞：＜语句＞
＜缺省＞   ::=  default : ＜语句＞
＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
＜语句列＞   ::= ｛＜语句＞｝
＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
＜写语句＞    ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’| printf ‘(’＜表达式＞‘)’
＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
附加说明：
（1）char类型的表达式，用字符的ASCII码对应的整数参加运算，在写语句中输出字符
（2）标识符区分大小写字母
（3）写语句中的字符串原样输出
（4）情况语句中，switch后面的表达式和case后面的常量只允许出现int和char类型；每个情况子语句执行完毕后，不继续执行后面的情况子语句
（5）数组的下标从0开始
（6）for语句先执行一次循环体中的语句再进行循环变量是否越界的测试
**/
