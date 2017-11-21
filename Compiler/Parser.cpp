#include <vector>
#include "Symbol.h"
#include "Parser.h"
using namespace std;

Parser::Parser(Lexer &_lex, ErrorHandler &_eh, MiddleCode &_mc, SymbolTable &_rt):
    lex(_lex), eh(_eh), mc(_mc), rootTable(_rt), curItem(NULL), curTbl(NULL){}
/***
1. Call GetSym() to get next token if no back before;
2. else just set curToken to points to next token;
***/
void Parser::NextToken()
{
    if(tokens.empty() || curToken == tokens.end()-1)
    {
        Symbol tmp = lex.GetSym();
        tokens.push_back(tmp);
        if(tokens.size() == 1)
        {
            curToken = tokens.begin();
        }
        else
        {
            curToken++;
        }
    }
    else
    {
        curToken++;
    }
}
/**
1. return back n tokens because we may pre-get tokens to
*  choose branches in Syntax analysis;
2. actually n is only 1--3
**/
void Parser::BackToken(int n)
{
    curToken = curToken - n;
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
bool Parser::InsertTable(string name, TableItemType type, int value = 0)
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
                if(curToken->type() != NUM)
                {
                    SetError(12);
                    SkipUntil(SEMICOLON, COMMA);
                    continue;
                }
                if(!InsertTable(name, CONST_INT, curToken->numVal()))
                {
                    SetError(13, name);// 重复声明，不跳读，因为语法成分是对的
                }
                else
                {// gen middle code

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
                {

                }
                NextToken();
                continue;
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
    // into paralist
    ParaList();
    // gen middle code here: int/char funcname() *****

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
    curTbl = &rootTable;
    return;
}
/**
 * ＜参数表＞::=＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}|＜空＞
*/
void Parser::ParaList()
{
    int paraCount = 0;
    if(curToken->type() == Rpar)
    {
        curItem->SetParamCount(paraCount); // 无参数
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
            (curItem->paraName).push_back(name); // store para name in func item
            paraCount++;
            if(!InsertTable(name, tp))
            {
                SetError(13);
            }
            else
            {// gen middle code: para name(curItem)

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
    while(curToken->type() != Rbrac)
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
            if(curToken->type() == GIVEN)
            {
                BackToken(1);
                GiveState();
            }
            else if(curToken->type() == Lpar)
            {
                BackToken(1);
                CallFuncState();
            }
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





