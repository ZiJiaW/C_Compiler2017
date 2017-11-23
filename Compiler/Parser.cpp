#include <vector>
#include <cstdlib>
#include "Symbol.h"
#include "Parser.h"
using namespace std;

Parser::Parser(Lexer &_lex, ErrorHandler &_eh, MiddleCode &_mc, SymbolTable &_rt):
    lex(_lex), eh(_eh), mc(_mc), rootTable(_rt), curItem(NULL), curTbl(NULL), tempIndex(-1), curIndex(0){}
/***
1. Call GetSym() to get next token if no back before;
2. else just set curToken to points to next token;
***/
void Parser::NextToken()
{
    if(tokens.empty() || curIndex == tokens.size()-1)
    {
        Symbol* tmp = new Symbol(lex.GetSym());
        tokens.push_back(tmp);
        if(tokens.size() == 1)
        {
            curIndex = 0;
        }
        else
        {
            curIndex++;
        }
    }
    else
    {
        curIndex++;
    }
    curToken = tokens[curIndex];
    cout<<curToken->name()<<endl;
}
/**
1. return back n tokens because we may pre-get tokens to
*  choose branches in Syntax analysis;
2. actually n is only 1--3
**/
void Parser::BackToken(int n)
{
    curIndex = curIndex - n;
    curToken = tokens[curIndex];
}
/**
* skip tokens
**/
void Parser::SkipUntil(SymType tp)
{
    while(curToken->type() != tp)
    {
        NextToken();
    }
    NextToken();
}
void Parser::SkipUntil(SymType tp1, SymType tp2)
{
    while(curToken->type() != tp1 && curToken->type() != tp2)
    {
        NextToken();
    }
    NextToken();
}
bool Parser::SkipInlineUntil(SymType tp)
{
    while(curToken->type() != tp)
    {
        int pr = curToken->lineNum();
        NextToken();
        if(curToken->lineNum() != pr)
        {
            return false;
        }
    }
    NextToken();
    return true;
}
/**
1. set curTbl points to the rootTable;
2. get a new token for following analysis;
*/
void Parser::StartParsing()
{
    curTbl = &rootTable;
    NextToken();
    Program();
    return;
}
/**
1. insert a TableItem in curTbl;
2. points curItem to this item;
3. if exists, return false;
*/
bool Parser::InsertTable(string name, TableItemType type, int value)
{
    curItem = curTbl->Insert(name, type, value);
    if(!curItem)
        return false;
    else
    {
        if(type == INT_FUNC || type == CHAR_FUNC || type == VOID_FUNC)
        {// if function, new a child symboltable
         // set curTbl to this symboltable
            SymbolTable *t = new SymbolTable();
            curItem->SetField(t);
            curTbl = t;
        }
        return true;
    }
}
/**
＜程序＞::=［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
1. first 常量说明 = const
2. 后面需要预读
*/
void Parser::Program()
{
#ifdef DEBUG
    cout<<"Start parsing program"<<endl;
#endif // DEBUG
    if(curToken->type() == symconst)
    {
        ConstState(); // 常量说明
    }
    if(curToken->type() == symint || curToken->type() == symchar)
    {
        NextToken(); NextToken(); // pre-read two tokens
        if(curToken->type() != Lpar) // 变量说明
        {
            BackToken(2);
            VarState();
        }
        else
        {
            BackToken(2);  // 否则是函数定义，外面处理
        }
    }
    // error if not function type
    while(!(curToken->type() == symint || curToken->type() == symchar || curToken->type() == symvoid))
    {
        SetError(7);
        SkipUntil(SEMICOLON, Rbrac); // skip to ; or }
    }
    // here token may be int, char, void function
    // pre-read one token to judge if it's main;
    bool aftermain = false;
    while(curToken->type() == symint || curToken->type() == symchar || curToken->type() == symvoid)
    {
        if(curToken->type() == symvoid)
        {
            NextToken();
            if(curToken->type() == symmain)
            {
                BackToken(1);
                MainFunc();
                aftermain = true;
            }
            else
            {
                BackToken(1);
                FuncWithoutRet();
            }
        }
        else
        {
            FuncWithRet();
        }
    }
    if(!aftermain)
    {
        SetError(8);
    }
    else if(curToken->type() != END)
    {
        SetError(9);
    }
    return;
}
/******
＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
                |   char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
*****/
void Parser::ConstState()
{
    // const has been confirmed outside
    while(curToken->type() == symconst)
    {
        NextToken();
        // expect int/char
        if(!(curToken->type() == symint || curToken->type() == symchar))
        {
            SetError(10);
            SkipUntil(SEMICOLON);
            continue;
        }
        // const int a = 10, b = 2;
        if(curToken->type() == symint)
        {
            #ifdef DEBUG
            cout<<"this is a const int statement"<<endl;
            #endif // DEBUG
            NextToken();
            if(curToken->type() != IDENT)
            {
                SetError(11);
                SkipUntil(SEMICOLON);
                continue;
            }
            while(curToken->type() == IDENT)
            {
                string name = curToken->name();
                NextToken();
                if(curToken->type() != GIVEN)
                {
                    SetError(6);
                    SkipUntil(SEMICOLON, COMMA); // skip until , or ;
                    continue;
                }
                NextToken();
                // 进入整数部分 const int a = +12,b = -23, c= 20, d=0;
                int num; // 记录整数常量的值
                if(curToken->type() == NUM)
                {
                    num = curToken->numVal();
                }
                else if(curToken->type() == PLUS || curToken->type() == MINUS)
                {
                    bool plus = curToken->type() == PLUS;
                    NextToken();
                    if(curToken->type() != NUM)
                    {
                        SetError(12);
                        SkipUntil(SEMICOLON, COMMA);
                        continue;
                    }
                    num = plus ? curToken->numVal() : -curToken->numVal();
                }
                else
                {
                    SetError(12);
                    SkipUntil(SEMICOLON, COMMA);
                    continue;
                }
                if(!InsertTable(name, CONST_INT, num))
                {
                    SetError(13, name);// 重复声明，不跳读，因为语法成分是对的
                }
                else
                {// gen middle code
                    mc.Generate(STR, curItem);
                }
                NextToken();
                if(curToken->type() == COMMA)
                {
                    NextToken();
                    continue;
                }
                else if(curToken->type() == SEMICOLON)
                {
                    NextToken();
                    break;
                }
                else
                {
                    SetError(14);
                    SkipUntil(SEMICOLON);
                    break;
                }
            }
        }
        // const char x = 'c', y = 'k';
        else
        {
            #ifdef DEBUG
            cout<<"this is a const char statement"<<endl;
            #endif // DEBUG
            NextToken();
            if(curToken->type() != IDENT)
            {
                SetError(11); // 找不到标志符
                SkipUntil(SEMICOLON);
                continue;
            }
            while(curToken->type() == IDENT)
            {
                string name = curToken->name();
                NextToken();
                if(curToken->type() != GIVEN)
                {
                    SetError(6);
                    SkipUntil(SEMICOLON, COMMA); // skip until , or ;
                    continue;
                }
                NextToken();
                if(curToken->type() != constch)
                {
                    SetError(15);
                    SkipUntil(SEMICOLON, COMMA);
                    continue;
                }
                if(!InsertTable(name, CONST_CHAR, curToken->numVal()))
                {
                    SetError(13, name);
                }
                else
                {// gen middle code
                    mc.Generate(STR, curItem);
                }
                NextToken();
                if(curToken->type() == COMMA)
                {
                    NextToken();
                    continue;
                }
                else if(curToken->type() == SEMICOLON)
                {
                    NextToken();
                    break;
                }
                else
                {
                    SetError(14);
                    SkipUntil(SEMICOLON);
                    break;
                }
            }
        }
    }
    return;
}

/***
＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’}
curToken is int/char(confirmed outside)
***/
void Parser::VarState()
{
    while(curToken->type() == symint || curToken->type() == symchar)
    {
        // 还需要做函数和变量声明的判别
        NextToken(); NextToken();
        if(curToken->type() == Lpar) // 到达函数部分，退出
        {
            BackToken(2);
            break;
        }
        BackToken(2);
        SymType nowtype = curToken->type(); // int/char a, b[10], c;
        NextToken();
        if(curToken->type() != IDENT)
        {
            SetError(11);
            SkipUntil(SEMICOLON);// 暂时处理的较粗，直接跳过整行定义
            continue;
        }
        while(curToken->type() == IDENT)
        {
            string name = curToken->name();
            NextToken();
            if(curToken->type() == COMMA) // int/char a,
            {
                if(!InsertTable(name, nowtype==symint ? INT : CHAR, 0))// 自动初始化为0
                {
                    SetError(13); // 重复定义
                }
                // todo: gen middle code here
                mc.Generate(STR, curItem);
                NextToken();
                continue;
            }
            else if(curToken->type() == SEMICOLON)
            {
                if(!InsertTable(name, nowtype==symint ? INT : CHAR, 0))
                {
                    SetError(13);
                }
                mc.Generate(STR, curItem);
                NextToken();
                break;;
            }
            else if(curToken->type() == Lspar) // int/char a[10] ,/;
            {
                NextToken();
                if(curToken->type() != NUM)
                {
                    SetError(16); // 无效的数组长度
                    if(!SkipInlineUntil(Rspar))// can't find a ] in line
                    {
                        break;
                    }
                    // if find a ] in line, now may be a , or ;
                }
                else
                {
                    int arraysize = curToken->numVal();
                    if(arraysize == 0)
                    {
                        SetError(16);
                    }
                    else
                    {
                        if(!InsertTable(name, nowtype==symint ? INT_ARR : CHAR_ARR))
                        {
                            SetError(13);
                        }
                        else
                        {
                            curItem->SetLength(arraysize);
                            // todo: gen middle code
                            {
                                mc.Generate(STR, curItem);
                            }
                        }
                    }
                    NextToken();
                    if(curToken->type() != Rspar)
                    {
                        SetError(17);
                        if(!SkipInlineUntil(COMMA))
                        {
                            break;
                        }
                        continue;
                    }
                    NextToken();
                }
                // may get a , or ;
                if(curToken->type() == COMMA)
                {
                    NextToken();
                    continue;
                }
                else if(curToken->type() == SEMICOLON)
                {
                    NextToken();
                    break;
                }
                else
                {
                    SetError(6);
                    SkipUntil(SEMICOLON);
                    break;
                }
            }
        }
    }
    return;
}
/**
 * ＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
 * ＜声明头部＞   ::=  int＜标识符＞ |char＜标识符＞
 * ＜参数＞    ::= ＜参数表＞
 * ＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
 * ＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
 * ＜语句列＞   ::= ｛＜语句＞｝
 * ＜语句＞    ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;
 *  | ＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
 * int or char has been confirmed outside;
 * must return sth at least once!!
*/
void Parser::FuncWithRet()
{
    TableItemType funcType = curToken->type() == symint ? INT_FUNC : CHAR_FUNC;
    NextToken();
    if(curToken->type() != IDENT)
    {
        SetError(11);
        SkipUntil(Rbrac);
        return;
    }
    string name = curToken->name();
    if(!InsertTable(name, funcType))
    {
        SetError(13);
        SkipUntil(Rbrac);
        return;
    }
    // now curTbl points to this func's field
    // curItem points to this func's item
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Rbrac);
        curTbl = &rootTable; // restore curTbl
        return;
    }
    NextToken();
    // gen middle code here: int/char funcname() *****
    mc.Generate(FUNC, curItem);
    curFunc = curItem;
    // into paralist
    ParaList(curItem);
    // now ) next {
    if(curToken->type() != Rpar)
    {
        SetError(19);
        SkipUntil(Lbrac);
    }
    else
    {
        NextToken();
        if(curToken->type() != Lbrac)
        {
            SetError(20);
            SkipUntil(Rbrac);
            curTbl = &rootTable;
            return;
        }
        NextToken();
    }
    ComplexSentence();
    if(curToken->type() != Rbrac)
    {
        SetError(21);
        curTbl = &rootTable;
        return;
    }
    NextToken();
    // should judge if this function has returned sth
    if(!curTbl->funcWithRetValid())
    {
        SetError(23, name); // just set an error
    }
    curTbl = &rootTable;
    return;
}
/**
 * ＜参数表＞::=＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
*/
void Parser::ParaList(TableItem* funcItem)
{
    int paraCount = 0;
    if(curToken->type() == Rpar)
    {
        funcItem->SetParamCount(paraCount); // 无参数
        return;
    }
    else
    {
        while(true)// int a, char b, int c)
        {
            if(curToken->type() != symint && curToken->type() != symchar)
            {
                SetError(10);
                if(!SkipInlineUntil(COMMA))
                {
                    break;// next: {
                }
                else
                {
                    continue;
                }
            }
            TableItemType tp = curToken->type() == symint ? INT : CHAR;
            NextToken();
            if(curToken->type() != IDENT)
            {
                SetError(11);
                SkipUntil(Rpar);
                break;
            }
            string name = curToken->name();
            (funcItem->paraName).push_back(name); // store para name in func item
            paraCount++;
            if(!InsertTable(name, tp))
            {
                SetError(13);
            }
            else
            {// gen middle code: para name(curItem)
                mc.Generate(PARA, curItem);
            }
            NextToken();
            if(curToken->type() == COMMA)
            {
                NextToken();
                continue;
            }
            else
            {
                break;
            }
        }
    }
    funcItem->SetParamCount(paraCount);
    return;
}

/**
 * ＜复合语句＞::=［＜常量说明＞］［＜变量说明＞］＜语句列＞
 * ＜语句列＞ ::=｛＜语句＞｝
 * ＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;
 *           |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
 * first set: if, for, {, IDENT, printf, scanf, switch, return, ;
*/
void Parser::ComplexSentence()
{
    if(curToken->type() == symconst)
    {
        ConstState(); // 常量说明
    }
    if(curToken->type() == symint || curToken->type() == symchar)
    {
        NextToken(); NextToken(); // pre-read two tokens
        if(curToken->type() != Lpar) // 变量说明
        {
            BackToken(2);
            VarState();
        }
        else
        {
            BackToken(2);
        }
    }
    // 语句列
    SentenceList();
    return;
}
void Parser::SentenceList()
{
    while(curToken->type() == symif || curToken->type() == symfor || curToken->type() == Lbrac
        ||curToken->type() == IDENT || curToken->type() == symprintf || curToken->type() == symscanf
        || curToken->type() == symswitch || curToken->type() == symreturn || curToken->type() == SEMICOLON)
    {
        Sentence();
    }
    return;
}
/**
 * ＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞;
 * |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
 * first set: if, for, {, IDENT, printf, scanf, switch, return, ;
*/
void Parser::Sentence()
{
    switch(curToken->type())
    {
        case symif:
        {
            IfState();
            return;
        }
        case symfor:
        {
            ForState();
            return;
        }
        case Lbrac:
        {
            NextToken();
            SentenceList();
            if(curToken->type() != Rbrac)
            {
                SetError(21);
                SkipInlineUntil(Rbrac);
            }
            else
            {
                NextToken();
            }
            return;
        }
        // 有分号的都在switch之外处理，没有分号的均直接return
        case IDENT:// 可能是函数调用或是赋值，预读
        {
            NextToken();
            if(curToken->type() == Lpar)
            {
                BackToken(1);
                CallFuncState();
            }
            else
            {
                BackToken(1);
                GiveState();
            }
            break;
        }
        case symprintf:
        {
            PrintState();
            break;
        }
        case symscanf:
        {
            ScanfState();
            break;
        }
        case symswitch:
        {
            SwitchState();
            return;
        }
        case symreturn:
        {
            ReturnState();
            break;
        }
        case SEMICOLON:
        {
            break;
        }
        default:
        {
            SetError(6);
            SkipUntil(SEMICOLON, Rbrac);
            return;
        }
    }
    if(curToken->type() == SEMICOLON)
    {
        NextToken();
    }
    else
    {
        SetError(22);
        SkipUntil(SEMICOLON);
    }
    return;
}
/**
 * ＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
*/
void Parser::FuncWithoutRet()
{
    TableItemType funcType = VOID_FUNC;
    NextToken();
    if(curToken->type() != IDENT)
    {
        SetError(11);
        SkipUntil(Rbrac);
        return;
    }
    string name = curToken->name();
    if(!InsertTable(name, funcType))
    {
        SetError(13);
        SkipUntil(Rbrac);
        return;
    }
    // now curTbl points to this func's field
    // curItem points to this func's item
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Rbrac);
        curTbl = &rootTable; // restore curTbl
        return;
    }
    NextToken();
    // gen middle code here: int/char funcname() *****
    mc.Generate(FUNC, curItem);
    curFunc = curItem;
    // into paralist
    ParaList(curItem);
    // now ) next {
    if(curToken->type() != Rpar)
    {
        SetError(19);
        SkipUntil(Lbrac);
    }
    else
    {
        NextToken();
        if(curToken->type() != Lbrac)
        {
            SetError(20);
            SkipUntil(Rbrac);
            curTbl = &rootTable;
            return;
        }
        NextToken();
    }
    ComplexSentence();
    if(curToken->type() != Rbrac)
    {
        SetError(21);
        curTbl = &rootTable;
        return;
    }
    NextToken();
    // should judge if this function has returned sth
    if(!curTbl->funcWithoutRetValid())
    {
        SetError(24, name); // just set an error
    }
    mc.Generate(END_FUNC, curFunc);
    curTbl = &rootTable;
    return;
}
/**
 * ＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
 * ident void or main has been confirmed outside
*/
void Parser::MainFunc()
{
    NextToken();
    NextToken();
    // gen middle code here: FUNC main
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Lbrac);
    }
    else
    {
        NextToken();
        if(curToken->type() != Rpar)
        {
            SetError(19);
            SkipUntil(Lbrac);
        }
        else
        {
            NextToken();
            if(curToken->type() != Lbrac)
            {
                SetError(20);
            }
            else
            {
                NextToken();
            }
        }
    }
    if(!InsertTable("main", VOID_FUNC))
    {
        SetError(13, "main");
    }
    else
    {
        curItem->SetParamCount(0);
        // gen middle code here: FUNC main
        mc.Generate(FUNC, curItem);
    }
    TableItem* nowfunc = curItem; // 保存当前函数原型的符号表指针
    ComplexSentence();
    if(curToken->type() != Rbrac)
    {
        SetError(21);
    }
    else if(!curTbl->funcWithoutRetValid())
    {
        SetError(24, "main");
    }
    else
    {// gen middle code: end of program
        mc.Generate(END_PROC, nowfunc);
    }
    NextToken();
}
/**
 * ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
*/
TableItem* Parser::Expression()
{
    bool gotminus = false;
    if(curToken->type() == PLUS)
    {
        NextToken();
    }
    else if(curToken->type() == MINUS)
    {
        gotminus = true;
        NextToken();
    }
    TableItem* tmp1 = Term();
    if(gotminus)
    {
        TableItem* tmp2 = NewTmpVal();
        mc.Generate(NEG, tmp2, tmp1);
    }
    while(curToken->type() == PLUS || curToken->type() == MINUS)
    {
        OpCode op = curToken->type() == PLUS ? ADD : SUB;
        TableItem* src1 = TempTable[tempIndex];
        NextToken();
        TableItem* src2 = Term();
        if(!src2)
            return NULL;
        TableItem* dst = NewTmpVal();
        mc.Generate(op, dst, src1, src2);
    }
    return TempTable[tempIndex];
}
/**
 * ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
*/
TableItem* Parser::Term()
{
    Factor();
    if(!(curToken->type() == TIMES || curToken->type() == DIVIDE))
        return TempTable[tempIndex];
    while(curToken->type() == TIMES || curToken->type() == DIVIDE)
    {
        OpCode op = curToken->type() == TIMES ? MUL : DIV;
        TableItem* src1 = TempTable[tempIndex];
        NextToken();
        TableItem* src2 = Factor();
        if(!src2)
            return NULL;
        TableItem* dst = NewTmpVal();
        mc.Generate(op, dst, src1, src2);
    }
    return TempTable[tempIndex];
}
/**
 * ＜因子＞::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
 * first: IDENT, +-, NUM, constch, (
*/
TableItem* Parser::Factor()
{
    if(curToken->type() == IDENT) // a, a[10], sum(a, b)
    {
        string name = curToken->name();
        NextToken();
        if(curToken->type() == Lspar) // a[10*2]
        {
            TableItem* src1 = GetItemByName(name);
            if(!src1)
            {
                SetError(25, name);
                return NULL;
            }
            else if(src1->type() != INT_ARR && src1->type() != CHAR_ARR)
            {
                SetError(26, name);
                return NULL;
            }
            TableItem* src2 = Expression();
            if(!src2)
                return NULL;
            TableItem* dst = NewTmpVal();
            mc.Generate(ARR_GIV, dst, src1, src2);
            if(curToken->type() != Rspar)
            {
                SetError(17);
                return NULL;
            }
            NextToken();
            return dst;
        }
        else if(curToken->type() == Lpar) // sum()
        {
            BackToken(1);
            // TODO: 外面判断一下函数是否存在以及类型是否符合
            TableItem* t = GetItemByName(curToken->name());
            if(!t)
            {
                SetError(25);
                return NULL;
            }
            else if(t->type() != INT_FUNC && t->type() != CHAR_FUNC)
            {
                SetError(26);
                return NULL;
            }
            return CallFuncState();//函数调用语句，内部设置临时变量，并生成返回的中间代码
        }
        else // a
        {
            TableItem* t = GetItemByName(name);
            if(!t)
            {
                SetError(25, name);
                return NULL;
            }
            else if(t->type() == CONST_INT || t->type() == CONST_CHAR ||
                    t->type() == INT || t->type() == CHAR)
            {
                TableItem* dst = NewTmpVal(); // 这里多做一次赋值，为了一致性
                mc.Generate(GIV, dst, t);
                return dst;
            }
            else
            {
                SetError(26, name);
                return NULL;
            }
        }
    }
    else if(curToken->type() == constch)
    {
        TableItem* t = NewTmpVal(char(curToken->numVal()));
        NextToken();
        return t;
    }
    else if(curToken->type() == NUM || curToken->type() == PLUS || curToken->type() == MINUS)
    {
        bool isminus = curToken->type() == MINUS;
        if(curToken->type() == NUM)
        {
            TableItem* t = NewTmpVal(curToken->numVal());
            NextToken();
            return t;
        }
        else
        {
            NextToken();
            if(curToken->type() != NUM)
            {
                SetError(12);
                NextToken();
                return NULL;
            }
            TableItem* t = NewTmpVal(isminus?-curToken->numVal():curToken->numVal());
            NextToken();
            return t;
        }
    }
    else if(curToken->type() == Lpar)
    {
        NextToken();
        TableItem* t = Expression();
        if(curToken->type() != Rpar)
        {
            SetError(19);
            return NULL;
        }
        NextToken();
        return t;
    }
    else
    {
        SetError(11);
        return NULL;
    }
}
/**
 * ＜标识符＞‘(’＜值参数表＞‘)’
 * ＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
 * if void or error, return NULL;
 * if int/char, return tmptable item pointer;
*/
TableItem* Parser::CallFuncState()// sum(a+2, b)
{
    TableItem* func = GetItemByName(curToken->name());// 获取该函数的符号表项
    if(!func)
    {
        SetError(25);
        SkipUntil(Rpar);
        return NULL;
    }
    else if(func->type() != VOID_FUNC && func->type() != INT_FUNC && func->type() != CHAR_FUNC)
    {
        SetError(26);
        SkipUntil(Rpar);
        return NULL;
    }
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        return NULL;
    }
    NextToken();
    int pcount = 0;
    while(curToken->type() == IDENT)
    {
        TableItem *para = Expression();
        mc.Generate(PUSH, para);
        pcount++;
        // now , or )
        if(curToken->type() == COMMA)
        {
            NextToken();
            continue;
        }
        else if(curToken->type() == Rpar)
        {
            break;
        }
    }
    if(curToken->type() != Rpar)
    {
        SetError(19);
        return NULL;
    }
    if(pcount != func->paramCount())
    {
        SetError(27, func->name());
        return NULL;
    }
    mc.Generate(CALL, func);
    if(func->type() == VOID_FUNC)
        return NULL;
    TableItem* result = NewTmpVal();
    mc.Generate(GET_RET, result);
    NextToken();
    return result;
}
/**
 * ＜赋值语句＞ ::=＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
 * like: a = 2*8*b  |  a[2*5] = 3+5
*/
void Parser::GiveState()
{
    TableItem* dst = GetItemByName(curToken->name()); // 被赋值对象的符号表项
    if(!dst)
    {
        SetError(25);
        SkipUntil(SEMICOLON);
        BackToken(1);
        return;
    }
    if(dst->type() == INT || dst->type() == CHAR)
    {
        NextToken();
        if(curToken->type() != GIVEN)
        {
            SetError(29);
            SkipUntil(SEMICOLON);
            BackToken(1);
            return;
        }
        NextToken();
        TableItem* src = Expression();
        mc.Generate(GIV, dst, src);
        return;
    }
    else if(dst->type() == INT_ARR || dst->type() == CHAR_ARR)
    {
        NextToken();
        if(curToken->type() != Lspar)
        {
            SetError(30);
            SkipUntil(SEMICOLON);
            BackToken(1);
            return;
        }
        NextToken();
        TableItem* src1 = Expression();
        if(curToken->type() != Rspar)
        {
            SetError(17);
            SkipUntil(SEMICOLON);
            BackToken(1);
            return;
        }
        NextToken();
        if(curToken->type() != GIVEN)
        {
            SetError(29);
            SkipUntil(SEMICOLON);
            BackToken(1);
            return;
        }
        NextToken();
        TableItem* src2 = Expression();
        mc.Generate(GIV_ARR, dst, src1, src2); //dst[src1] = src2
        return;
    }
    else
    {
        SetError(28);
        SkipUntil(SEMICOLON);
        BackToken(1);
        return;
    }
}

/**
 * ＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
 * need to record curTbl's returnsth, returnnth value;
 * first sym symreturn has been judged outside;
*/
void Parser::ReturnState()// return (2*3) or return
{
    NextToken();
    if(curToken->type() != Lpar)
    {
        mc.Generate(RET);
        curTbl->HasReturnedNth();
        return;
    }
    NextToken();
    TableItem* dst = Expression();
    if(curToken->type() != Rpar)
    {
        SetError(19);
        SkipUntil(SEMICOLON);
        BackToken(1);
        return;
    }
    mc.Generate(RET, dst);
    curTbl->HasReturnedSth();
    NextToken();
    return;
}
/**
 * ＜读语句＞ ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’ scanf(a, b);
*/
void Parser::ScanfState()
{
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Rpar);
        return;
    }
    NextToken();
    while(true)
    {
        if(curToken->type() != IDENT)
        {
            SetError(27, "scanf");
            SkipUntil(Rpar);
            return;
        }
        TableItem* dst = GetItemByName(curToken->name());
        if(!dst)
        {
            SetError(25);
            SkipUntil(Rpar);
            return;
        }
        if(dst->type() != INT && dst->type() != CHAR)
        {
            SetError(27, "scanf");
            SkipUntil(Rpar);
            return;
        }
        mc.Generate(READ, dst);
        NextToken();// , or )
        if(curToken->type() == Rpar)
            break;
        else if(curToken->type() == COMMA)
        {
            NextToken();
            continue;
        }
        else
        {
            SetError(11);
            SkipUntil(Rpar);
            return;
        }
    }
    NextToken();
    return;
}
/**
 * ＜写语句＞ ::= printf ‘(’ ＜字符串＞,＜表达式＞ ‘)’| printf ‘(’＜字符串＞ ‘)’| printf ‘(’＜表达式＞‘)’
 * print sym has been confirmed outside;
 * 若表达式是单个标识符或常量，不进入表达式分析（需要考虑类型输出）print(a)按a类型输出, print('a')输出字符
 * 可以进行类型记录（常量中间变量类型CONST，标识符符号表记录了类型）
 * 否则进入表达式计算，最终输出整数，中间变量类型TMP: print('a'+2) 输出整数
 * 当然生成的中间代码都是PRINT X，具体将在目标代码中进行判断
*/
void Parser::PrintState()
{
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Rpar);
        return;
    }
    NextToken();
    if(curToken->type() == conststr)
    {
        TableItem* dst = NewTmpVal(curToken->name());
        mc.Generate(WRITE, dst);
        NextToken();
        if(curToken->type() == Rpar)
        {
            NextToken();
            return;
        }
        else if(curToken->type() == COMMA)
        {
            NextToken();
            NextToken();
            if(curToken->type() == Rpar) // print("hello", 'a') or print("hello", 20) or print("hello", a)
            {
                BackToken(1);
                if(curToken->type() == NUM)
                {
                    TableItem* dst = NewTmpVal(curToken->numVal());
                    mc.Generate(WRITE, dst);
                }
                else if(curToken->type() == constch)
                {
                    TableItem* dst = NewTmpVal(char(curToken->numVal()));
                    mc.Generate(WRITE, dst);
                }
                else if(curToken->type() == IDENT)
                {
                    TableItem* dst = GetItemByName(curToken->name());
                    if(!dst)
                    {
                        SetError(25);
                    }
                    else if(dst->type() != INT && dst->type() != CHAR &&
                            dst->type() != CONST_INT && dst->type() != CONST_CHAR &&
                            dst->type() != INT_ARR && dst->type() != CHAR_ARR)
                    {
                        SetError(27);
                    }
                    mc.Generate(WRITE, dst);
                }
                else
                {
                    SetError(11);
                }
                NextToken(); // now )
            }
            else          // print("hello", 'a'+2)
            {
                BackToken(1);
                TableItem* dst = Expression();
                mc.Generate(WRITE, dst); // now )
            }
            NextToken();
            return;
        }
        else
        {
            SetError(11);
            SkipUntil(SEMICOLON);
            BackToken(1);
            return;
        }
    }
    else // print('a'+2) print('a')
    {
        NextToken();
        if(curToken->type() == Rpar) // print('a') or print(20) or print(a)
        {
            BackToken(1);
            if(curToken->type() == NUM)
            {
                TableItem* dst = NewTmpVal(curToken->numVal());
                mc.Generate(WRITE, dst);
            }
            else if(curToken->type() == constch)
            {
                TableItem* dst = NewTmpVal(char(curToken->numVal()));
                mc.Generate(WRITE, dst);
            }
            else if(curToken->type() == IDENT)
            {
                TableItem* dst = GetItemByName(curToken->name());
                if(!dst)
                {
                    SetError(25);
                }
                else if(dst->type() != INT && dst->type() != CHAR &&
                        dst->type() != CONST_INT && dst->type() != CONST_CHAR &&
                        dst->type() != INT_ARR && dst->type() != CHAR_ARR)
                {
                    SetError(27);
                }
                mc.Generate(WRITE, dst);
            }
            else
            {
                SetError(11);
            }
            NextToken(); // now )
        }
        else          // print('a'+2)
        {
            BackToken(1);
            TableItem* dst = Expression();
            mc.Generate(WRITE, dst); // now )
            if(curToken->type() != Rpar)
            {
                SetError(19);
            }
        }
        NextToken();
        return;
    }
}
/**
 * generate a new name for temp varible in expression
*/
string Parser::GetTempName()
{
    tempIndex++;
    char buffer[10];
    itoa(tempIndex, buffer, 10);
    return string("t") + string(buffer);
}
TableItem* Parser::NewTmpVal()
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, TMP);//临时变量
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::NewTmpVal(int constval)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, CONST_INT, constval);//整型常量
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::NewTmpVal(char constchar)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, CONST_CHAR, constchar);//字符常量
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::NewTmpVal(string conststr)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, CONST_STR);//字符串常量
    // 去除字符串开始和结尾的双引号
    conststr.erase(conststr.end()-1);
    conststr.erase(conststr.begin());
    tmp->paraName.push_back(conststr);
    TempTable.push_back(tmp);
    return tmp;
}
/**
 * get item pointer by name;
 * it will search item bottom-uply;
 * if can't find, return NULL;
*/
TableItem* Parser::GetItemByName(string name)
{
    if(curTbl == &rootTable)
        return curTbl->GetItem(name);
    else
    {
        TableItem* t = curTbl->GetItem(name);
        return t?t:rootTable.GetItem(name);
    }
}


void Parser::IfState(){}
void Parser::Condition(){}
void Parser::ForState(){}
void Parser::SwitchState(){}
void Parser::CaseList(){}
void Parser::CaseState(){}
void Parser::DefaultState(){}

