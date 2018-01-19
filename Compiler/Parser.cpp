#include <vector>
#include <cstdlib>
#include "Symbol.h"
#include "Parser.h"
using namespace std;

Parser::Parser(Lexer &_lex, ErrorHandler &_eh, MiddleCode &_mc, SymbolTable &_rt):
    lex(_lex), eh(_eh), mc(_mc), rootTable(_rt), curItem(NULL), curTbl(NULL), tempIndex(-1), curIndex(0)
{
    constZero = new TableItem("constZero", CONST_INT, 0);
}
Parser::~Parser()
{
    for(vector<TableItem*>::iterator p = TempTable.begin(); p != TempTable.end(); p++)
        delete *p;
    for(vector<Symbol*>::iterator p = tokens.begin(); p !=tokens.end(); p++)
        delete *p;
    delete constZero;
}
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
//    cout<<curToken->name()<<endl;
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
    #ifdef DEBUG
        cout<<"Start Parsing!"<<endl;
    #endif
    curTbl = &rootTable;
    NextToken();
    //cout<<curToken->name()<<endl;
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
    cout<<"This is program entry!"<<endl;
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
    #ifdef DEBUG
        cout<<"This is const statement!"<<endl;
    #endif
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
        #ifdef DEBUG
            cout<<"This is variable statement!"<<endl;
        #endif // DEBUG
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
#ifdef DEBUG
    cout<<"This is int/char function definition!"<<endl;
#endif // DEBUG
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
    mc.Generate(END_FUNC, curFunc);
    curTbl = &rootTable;
    return;
}
/**
 * ＜参数表＞::=＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
*/
void Parser::ParaList(TableItem* funcItem)
{
#ifdef DEBUG
    cout<<"Now in parameter list!"<<endl;
#endif // DEBUG
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
#ifdef DEBUG
    cout<<"Now in complex sentences!"<<endl;
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
#ifdef DEBUG
    cout<<"This is a void function definition!"<<endl;
#endif // DEBUG
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
#ifdef DEBUG
    cout<<"This is main function!"<<endl;
#endif // DEBUG4
    curTbl = &rootTable;
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
    curFunc = curItem; // 保存当前函数原型的符号表指针
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
        mc.Generate(END_PROC, curFunc);
    }
    NextToken();
}
/**
 * ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
*/
TableItem* Parser::Expression()
{
#ifdef DEBUG
    cout<<"This is an expression!"<<endl;
#endif // DEBUG
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
            NextToken();
            TableItem* src2 = Expression();
            if(!src2)
                return NULL;
            TableItemType tp = src1->type() == CHAR_ARR ? CHAR : TMP;
            TableItem* dst = NewTmpVal(tp);
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
                TableItemType tp = t->type() == CHAR || t->type() == CONST_CHAR ? CHAR : TMP;
                TableItem* dst = NewTmpVal(tp); // 这里多做一次赋值，为了一致性
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
#ifdef DEBUG
    cout<<"This is function call state"<<endl;
#endif // DEBUG
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
    while(true)
    {
        if(curToken->type() == Rpar)
        {
            break;
        }
        TableItem *para = Expression();
        mc.Generate(PUSH, para);
        pcount++;
        // now , or )
        if(curToken->type() == COMMA)
        {
            NextToken();
            continue;
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
    NextToken();
    if(func->type() == VOID_FUNC)
        return NULL;
    TableItemType tp = func->type() == CHAR_FUNC ? CHAR : TMP;
    TableItem* result = NewTmpVal(tp);
    mc.Generate(GET_RET, result);
    return result;
}
/**
 * ＜赋值语句＞ ::=＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
 * like: a = 2*8*b  |  a[2*5] = 3+5
*/
void Parser::GiveState()
{
#ifdef DEBUG
    cout<<"This is an assignment!"<<endl;
#endif // DEBUG
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
#ifdef DEBUG
    cout<<"This is return statement!"<<endl;
#endif // DEBUG
    NextToken();
    if(curToken->type() != Lpar)
    {
        mc.Generate(RET);
        mc.Generate(END_FUNC, curFunc);
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
    mc.Generate(END_FUNC, curFunc);
    curTbl->HasReturnedSth();
    NextToken();
    return;
}
/**
 * ＜读语句＞ ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’ scanf(a, b);
*/
void Parser::ScanfState()
{
#ifdef DEBUG
    cout<<"This is a scanf statement!"<<endl;
#endif // DEBUG
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
#ifdef DEBUG
    cout<<"This is a printf statement!"<<endl;
#endif // DEBUG
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
        TableItem* dst1 = NewTmpVal(curToken->name());
        //mc.Generate(WRITE, dst);
        NextToken();
        if(curToken->type() == Rpar)
        {
            mc.Generate(WRITE, dst1);
            NextToken();
            return;
        }
        else if(curToken->type() == COMMA)
        {
            NextToken();
            NextToken();
            if(curToken->type() == Rpar) // print("hello", 'a') or print("hello", 20) or print("hello", a)
            {
                mc.Generate(WRITE, dst1);
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
                mc.Generate(WRITE, dst1);
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
TableItem* Parser::NewTmpVal(TableItemType tp)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, tp);//临时变量
    curTbl->temp.push_back(tmp);
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::NewTmpVal(int constval)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, CONST_INT, constval);//整型常量
    curTbl->temp.push_back(tmp);
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::NewTmpVal(char constchar)
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, CONST_CHAR, constchar);//字符常量
    curTbl->temp.push_back(tmp);
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
    curTbl->temp.push_back(tmp);
    TempTable.push_back(tmp);
    return tmp;
}
TableItem* Parser::GenLabel()
{
    string tmpname = GetTempName();
    TableItem* tmp = new TableItem(tmpname, LABEL); // 表中添加标签
    curTbl->temp.push_back(tmp);
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
/*
if(a == d)
    a = a + 1;
else
    d = d + 1;
how to translate to MIPS?
suppose a in $s0, b in $s1;
**************************
bne label $s0 $s1
addi $s0 $s0 1
j end
label: addi $s1 $s1 1
end:......
**************************
how to translate for?
for(i = 0; i < 10; i = i + 1)
    sum = sum + 1;
suppose sum in $s0, i in $s1;
**************************
li $s1 0             GIV i 0
loop:                SETL loop_1
add $s0 $s0 $s1      ADD sum sum 1
addi $s1 $s1 1       ADD i i 1
slti $t0 $s1 10      SLT t0 i 10
bne $t0 $0 loop      BNE t0 0 loop_1
**************************
*/
/**
 * ＜条件语句＞::=if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
 * ＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
*/
void Parser::IfState()
{
#ifdef DEBUG
    cout<<"This is an if statement"<<endl;
#endif // DEBUG
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(SEMICOLON, Rbrac);
        return;
    }
    NextToken();
    TableItem* label = GenLabel();
    MiddleCode* cdt = Condition(label, true);
    mc.Concat(cdt);
    if(curToken->type() != Rpar)
    {
        SetError(19);
        SkipUntil(SEMICOLON, Rbrac);
        return;
    }
    NextToken();
    Sentence();
    TableItem* end = GenLabel();
    mc.Generate(JMP, end);
    mc.Generate(SETL, label);
    if(curToken->type() == symelse)
    {
        NextToken();
        Sentence();
        mc.Generate(SETL, end);
        return;
    }
    else
    {
        mc.Generate(SETL, end);
        return;
    }
}
/**
 * ＜条件＞::=＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
 * 在外部生成Label作为参数，内部生成带跳转的中间代码
 * 传出生成的中间代码，因为跳转的位置需要在外部调整，不能直接生成
 * 如果是if语句(isIf = true)，需要在条件不满足时跳转，for语句则条件满足时跳转
*/
MiddleCode* Parser::Condition(TableItem* label, bool isIf)
{
#ifdef DEBUG
    cout<<"Now in condition statement!"<<endl;
#endif // DEBUG
    MiddleCode* ret = new MiddleCode();
    TableItem* src1 = Expression();
    if(curToken->type() == EQ || curToken->type() == LEQ || curToken->type() == GEQ
    || curToken->type() == GRT || curToken->type() == LES || curToken->type() == NEQ)
    {
        SymType tp = curToken->type();
        NextToken();
        TableItem* src2 = Expression();
        switch(tp)
        {
            case EQ:{ // src1 == src2
                if(isIf)
                    ret->Generate(BNE, label, src1, src2); // if src1 != src2, jmp
                else
                    ret->Generate(BEQ, label, src1, src2); // is src1 == src2, jmp
                return ret;
            }
            case LEQ:{ // src1<=src2
                TableItem* dst = NewTmpVal();
                ret->Generate(SUB, dst, src1, src2); // dst = src1 - src2
                if(isIf)
                    ret->Generate(BGTZ, label, dst); // if dst > 0, branch to label
                else
                    ret->Generate(BLEZ, label, dst); // if dst <= 0, branch to label
                return ret;
            }
            case GEQ:{ // src1>=src2
                TableItem* dst = NewTmpVal();
                ret->Generate(SUB, dst, src1, src2); // dst = src1 - src2
                if(isIf)
                    ret->Generate(BLTZ, label, dst); // if dst < 0, jmp
                else
                    ret->Generate(BGEZ, label, dst); // if dst >=0, jmp
                return ret;
            }
            case GRT:{// src1>src2
                TableItem* dst = NewTmpVal();
                ret->Generate(SUB, dst, src1, src2); // dst = src1 - src2
                if(isIf)
                    ret->Generate(BLEZ, label, dst); // if dst <= 0, jmp
                else
                    ret->Generate(BGTZ, label, dst); // if dst >0, jmp
                return ret;
            }
            case LES:{// src1<src2
                TableItem* dst = NewTmpVal();
                ret->Generate(SUB, dst, src1, src2); // dst = src1 - src2
                if(isIf)
                    ret->Generate(BGEZ, label, dst); // if dst >= 0, jmp
                else
                    ret->Generate(BLTZ, label, dst); // if dst < 0, jmp
                return ret;
            }
            case NEQ:{
                if(isIf)
                    ret->Generate(BEQ, label, src1, src2);
                else
                    ret->Generate(BNE, label, src1, src2);
                return ret;
            }
        }
    }
    else
    {
        if(isIf) // if src1 == 0, jmp
            ret->Generate(BEQ, label, src1, constZero);
        else// if src1 != 0, jmp
            ret->Generate(BNE, label, src1, constZero);
        return ret;
    }
}
/**
 * ＜循环语句＞ ::=  for‘(’＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞‘)’＜语句＞
 * ＜步长＞ ::= ＜非零数字＞｛＜数字＞｝就是无符号整数，切出来是NUM的就行了，但不能是0
*/
void Parser::ForState()
{
#ifdef DEBUG
    cout<<"This is a for statement!"<<endl;
#endif // DEBUG
    NextToken();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(SEMICOLON);
    }
    else
    {
        NextToken();
        if(curToken->type() != IDENT)
        {
            SetError(11);
            SkipUntil(SEMICOLON);
        }
        else
        {
            NextToken();
            if(curToken->type() != GIVEN)
            {
                SetError(29);
                SkipUntil(SEMICOLON);
            }
            else
            {
                BackToken(1);
                GiveState();
                if(curToken->type() != SEMICOLON)
                {
                    SetError(22);
                    SkipUntil(SEMICOLON);
                }
                NextToken();
            }
        }
    }
    TableItem* loop = GenLabel();
    mc.Generate(SETL, loop);
    //***********************************
    int conditionStart = curIndex;
    SkipUntil(SEMICOLON);//先跳过条件，记录条件的开始位置
    BackToken(1);
    //MiddleCode* cdt = Condition(loop);

    //*********************************
    OpCode op;
    TableItem* dst;
    TableItem* src1;
    TableItem* src2;
    if(curToken->type() != SEMICOLON)
    {
        SetError(22);
        SkipUntil(Rpar);
    }
    else
    {
        NextToken();
        // i = i +/- 1
        if(curToken->type() != IDENT)
        {
            SetError(11);
            SkipUntil(Rpar);
        }
        else
        {
            dst = GetItemByName(curToken->name());
            if(!dst)
            {
                SetError(25);
                SkipUntil(Rpar);
            }
            else if(dst->type() != INT && dst->type() != CHAR)
            {
                SetError(26);
                SkipUntil(Rpar);
            }
            else
            {
                NextToken();
                if(curToken->type() != GIVEN)
                {
                    SetError(29);
                    SkipUntil(Rpar);
                }
                else
                {
                    NextToken();
                    if(curToken->type() != IDENT)
                    {
                        SetError(11);
                        SkipUntil(Rpar);
                    }
                    else
                    {
                        src1 = GetItemByName(curToken->name());
                        if(!src1)
                        {
                            SetError(25);
                            SkipUntil(Rpar);
                        }
                        else if(src1->type() != INT && src1->type() != CHAR && src1->type() != CONST_INT && src1->type() != CONST_CHAR)
                        {
                            SetError(26);
                            SkipUntil(Rpar);
                        }
                        else
                        {
                            NextToken();
                            op = curToken->type() == PLUS ? ADD :
                                 curToken->type() == MINUS ? SUB : END_FUNC;
                            if(op == END_FUNC)
                            {
                                SetError(11);
                                SkipUntil(Rpar);
                            }
                            NextToken();
                            if(curToken->type() != NUM)
                            {
                                SetError(12);
                                SkipUntil(Rpar);
                            }
                            else
                            {
                                if(curToken->numVal() == 0)
                                    SetError(36, "");
                                src2 = NewTmpVal(curToken->numVal());
                                NextToken();
                                if(curToken->type() != Rpar)
                                {
                                    SetError(19);
                                    SkipUntil(Rpar);
                                }
                                else
                                    NextToken();
                            }
                        }
                    }
                }
            }
        }
    }
    Sentence();// 内容
    int endfor = curIndex; // 记录结束的位置
    mc.Generate(op, dst, src1, src2);//步长
    // 回退到条件起点，进行条件判断
    curIndex = conditionStart;
    curToken = tokens[curIndex];
    MiddleCode* cdt = Condition(loop);
    mc.Concat(cdt);//最后添加跳转判断
    // 返回到结束位置
    curIndex = endfor;
    curToken = tokens[curIndex];
}

/**
 * ＜情况语句＞::=switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
 * ＜情况表＞::=＜情况子语句＞{＜情况子语句＞}
 * ＜情况子语句＞::=case＜常量＞：＜语句＞
 * ＜缺省＞::=default :＜语句＞
*/
/*
switch(a+b)                 t0 = a + b
{                           bne t0 2 label2
    case 2: b = 5;          b = 5
    case 1: b = 10;         label2: bne t0 1 label3
    default: b = 0;         b = 10
}                           label3: b = 0
*/
void Parser::SwitchState()
{
#ifdef DEBUG
    cout<<"This is switch statement!"<<endl;
#endif // DEBUG
    NextToken();
    TableItem* ex;
    TableItem* end = GenLabel();
    if(curToken->type() != Lpar)
    {
        SetError(18);
        SkipUntil(Lbrac);
    }
    else
    {
        NextToken();
        ex = Expression();
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
                SkipUntil(Lbrac);
            }
            else
                NextToken();
        }
    }
    CaseList(ex, end);
    DefaultState();
    mc.Generate(SETL, end);
    if(curToken->type() != Rbrac)
    {
        SetError(21);
        SkipUntil(SEMICOLON, Rbrac);
        return;
    }
    NextToken();
}
/**
 * ＜情况表＞::=＜情况子语句＞{＜情况子语句＞}
 * ＜情况子语句＞::=case＜常量＞：＜语句＞ case -1  case 2 case 'a'
*/
void Parser::CaseList(TableItem* ex, TableItem* end_label)
{
    #ifdef DEBUG
        cout<<"This is case statement!"<<endl;
    #endif
    if(curToken->type() != symcase)
    {
        SetError(31);
        SkipUntil(symdefault);
        BackToken(1);
        return;
    }
    SymType constTp = noone;
    while(curToken->type() == symcase)
    {
        NextToken();
        bool isminus = false;
        if(curToken->type() == MINUS || curToken->type() == PLUS)
        {
            isminus = curToken->type() == MINUS;
            NextToken();
        }
        if(curToken->type() != NUM && curToken->type() != constch)
        {
            SetError(32);
            SkipUntil(symcase, symdefault);
            BackToken(1);
            return;
        }
        if(constTp == noone)
            constTp = curToken->type();
        else if(curToken->type() != constTp)
            SetError(33, "");
        TableItem* src1 = NewTmpVal(isminus?-curToken->numVal():curToken->numVal());
        TableItem* label = GenLabel();
        mc.Generate(BNE, label, src1, ex);
        NextToken();
        if(curToken->type() != COLON)
        {
            SetError(34);
            SkipUntil(symcase, symdefault);
            BackToken(1);
            return;
        }
        NextToken();
        Sentence();
        mc.Generate(JMP, end_label);
        mc.Generate(SETL, label);
    }
}
/**
 * ＜缺省＞::=default :＜语句＞
*/
void Parser::DefaultState()
{
    #ifdef DEBUG
        cout<<"This is default!"<<endl;
    #endif
    if(curToken->type() != symdefault)
    {
        SetError(35);
        SkipUntil(Rbrac);
        BackToken(1);
        return;
    }
    NextToken();
    if(curToken->type() != COLON)
    {
        SetError(34);
        SkipUntil(Rbrac);
        BackToken(1);
        return;
    }
    NextToken();
    Sentence();
}

