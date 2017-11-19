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
    if(tokens.empty() || curToken == tokens.rbegin())
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
1. set curTbl points to the rootTable;
2. get a new token for following analysis;
*/
void Parser::StartParsing()
{
    curTbl = &rootTable;
    NextToken();
    Program();
}

/**
＜程序＞::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
1. first 常量 = const
2. 变量和函数需要预读判断
*/
void Parser::Program()
{
    if(curToken->type() == symconst)
    {
        ConstState();
    }
    if(curToken->type() == symint || curToken->type() == symchar)
    {
        NextToken(); NextToken(); // pre-read two tokens
        if(curToken->type() == Lpar)
        {
            BackToken(2);

        }
    }
}



















