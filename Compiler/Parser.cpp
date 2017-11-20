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
        return true;
}
/**
������::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
1. first ���� = const
2. �����ͺ�����ҪԤ���ж�
*/
void Parser::Program()
{
#ifdef DEBUG
    cout<<"Start parsing program"<<endl;
#endif // DEBUG
    if(curToken->type() == symconst)
    {
        ConstState();
    }
    if(curToken->type() == symint || curToken->type() == symchar)
    {
        NextToken(); NextToken(); // pre-read two tokens
        if(curToken->type() != Lpar) // ����˵��
        {
            BackToken(2);
            VarState();
        }
        else
        {
            BackToken(2);  // ������Ǻ�������������֮�����洦��
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
}
/******
* ������˵���� ::= const���������壾;{ const���������壾;}
*���������壾  ::= int����ʶ��������������{,����ʶ��������������}
*                 |char����ʶ���������ַ���{,����ʶ���������ַ���}
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
            SetError(10); // ��Ч��const���ͣ�ֱ����������const
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
                SetError(11); // ��Ч��const��ʶ����ֱ����������const
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
                    SetError(13, name);// �ظ����壬����������Ϊ�﷨�ɷ��ǶԵ�
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
                SetError(11); // ��Ч��const��ʶ����ֱ����������const
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
                    SetError(13, name);// �ظ����壬����������Ϊ�﷨�ɷ��ǶԵ�
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
}




















