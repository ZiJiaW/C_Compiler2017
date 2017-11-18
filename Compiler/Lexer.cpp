#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include "Symbol.h"
#include "Lexer.h"
#include "ErrorHandler.h"
using namespace std;
/**
1. open source file and init map;
2. get first line;
3. get first character;
*/
Lexer::Lexer(string fileName, ErrorHandler &_errorHandler): errorHandler(_errorHandler)
{
    curIndex = -1;
    lineNum = 0;
    file.open(fileName.c_str(), ios::in);
    fileEmpty = !NewLine();
    NewChar();
    MapInit();
}

/**
1. get a newline to buffer and lineNum++;
2. return false means EOF;
3. skip empty line with lineNum recorded;
*/
bool Lexer::NewLine()
{
    if(file.eof())
        return false;
    do {
        getline(file, buffer);
        lineNum++;
    } while(buffer.empty()&&!file.eof());
    buffer.append(1, '\n');
    return true;
}
/**
1. get a new character to curChar and set curIndex to it;
2. curChar=='\0' means EOF;
*/
void Lexer::NewChar()
{
    if(file.eof())
    {
        curChar = '\0';
    }
    else if(curIndex == -1 || curIndex < int(buffer.size()-1))
    {
        curIndex++;
        curChar = buffer[curIndex];
    }
    else
    {
        if(!NewLine())
        {
            curChar = '\0';
        }
        else
        {
            curIndex = 0;
            curChar = buffer[curIndex];
        }
    }
}

/**
1. It's called when error happens.
2. It skips several characters until ';', isspace(curChar), '}', ']', ')', '\0';
*/
void Lexer::Skip()
{
    while(!(isspace(curChar) || curChar == ';' || curChar == '\0' ||
            curChar == '}' || curChar == ']' || curChar == ')'))
    {
        NewChar();
    }
}

/**
1. return a valid symbol;
2. It assumes we've done a NewChar() outside by default;
*/
Symbol Lexer::GetSym()
{
    // skip space ch, if end, return END
    while(isspace(curChar))
    {
        NewChar();
    }
    if(curChar == '\0')
    {
        return Symbol(END);
    }
    string token(1, curChar);
    // 标识符开头: a---Z or _
    if(isalpha(curChar) || curChar == '_')
    {
        while(true)
        {
            NewChar();
            if(isalnum(curChar) || curChar == '_')
            {
                token.append(1, curChar);
            }
            else
            {
                break;
            }
        }
        // got sth like [sum]
        map<string, SymType>::iterator it = symTypeMap.find(token);
        if(it == symTypeMap.end())
        {
            return Symbol(IDENT, lineNum, token);
        }
        else
        {
            return Symbol(it->second, lineNum, token);
        }
    }
    // 数字开头: 0--9
    else if(isdigit(curChar))
    {
        while(true)
        {
            NewChar();
            if(isdigit(curChar))
            {
                token.append(1, curChar);
            }
            else
            {
                break;
            }
        }
        // got sth like [0, 012, 235, 124]
        int val = atoi(token.c_str());
        if(token.size() != 1 && token[0] == '0')
        {
            errorHandler.SetError(lineNum, 1, token); // NO.1: num like 012
            return Symbol(ERR, lineNum, token, val);
        }
        else
        {
            return Symbol(NUM, lineNum, token, val);
        }
    }
    // 字符: 'a', '2'
    // <字符>::='<加法运算符>'|'<乘法运算符>'|'<字母>'|'<数字>'
    // <数字>::=０|<非零数字>
    // <字母>::= _|a|...|z|A|...|Z
    else if(curChar == '\'')
    {
        NewChar();
        if(curChar == '+' || curChar == '-' || curChar == '*' ||
            curChar == '/' || curChar == '_' || isalnum(curChar))
        {
            token.append(1, curChar);
            NewChar();
            if(curChar == '\'')
            {   // we got sth like 'a'
                token.append(1, curChar);
                NewChar();
                return Symbol(constch, lineNum, token, int(token[1]));
            }
        }
        // got error here;
        errorHandler.SetError(lineNum, 2, token);
        Skip();
        return Symbol(ERR, lineNum, token);
    }
    // 字符串: "hello world"
    // <字符串>::="{十进制编码为32,33,35-126的ASCII字符}"
    // 根据文法，不考虑转义字符
    else if(curChar == '\"')
    {
        bool gotInvalidCh = false;
        while(true)
        {
            NewChar();
            token.append(1, curChar);
            if(!(curChar != '\"' && curChar != '\n' && curChar != '\0'))
                break;
            if(!(curChar == 32 || curChar == 33 || (curChar >= 35 && curChar <= 126)))
            {
                gotInvalidCh = true;
            }
        }
        // token here maybe "hello world", "hello\n, "hello, "hello[backspace]"
        if(curChar == '\n' || curChar == '\0')
        {
            errorHandler.SetError(lineNum, 4, token);
            Skip();
            return Symbol(ERR, lineNum);
        }
        else if(gotInvalidCh)
        {
            errorHandler.SetError(lineNum, 3, token);
            Skip();
            return Symbol(ERR, lineNum);
        }
        else
        {
            NewChar();
            return Symbol(conststr, lineNum, token);
        }
    }
    // ==, !=, >=, <=, =, >, <
    else if(curChar == '=')
    {
        NewChar();
        if(curChar == '=')
        {
            token.append(1, curChar);
            NewChar();
            return Symbol(EQ, lineNum, token);
        }
        else
        {
            return Symbol(GIVEN, lineNum, token);
        }
    }
    else if(curChar == '!')
    {
        NewChar();
        if(curChar == '=')
        {
            token.append(1, curChar);
            NewChar();
            return Symbol(NEQ, lineNum, token);
        }
        else{
            errorHandler.SetError(lineNum, 5, token);
            Skip();
            return Symbol(ERR, lineNum);
        }
    }
    else if(curChar == '<')
    {
        NewChar();
        if(curChar == '=')
        {
            token.append(1, curChar);
            NewChar();
            return Symbol(LEQ, lineNum, token);
        }
        else
        {
            return Symbol(LES, lineNum, token);
        }
    }
    else if(curChar == '>')
    {
        NewChar();
        if(curChar == '=')
        {
            token.append(1, curChar);
            NewChar();
            return Symbol(GEQ, lineNum, token);
        }
        else
        {
            return Symbol(GRT, lineNum, token);
        }
    }
    // +-*/()[]{},:;
    else
    {
        map<string, SymType>::iterator it = symTypeMap.find(token);
        if(it != symTypeMap.end())
        {
            NewChar();
            return Symbol(it->second, lineNum, token);
        }
        else
        {
            errorHandler.SetError(lineNum, 6, token);
            Skip();
            return Symbol(ERR, lineNum);
        }
    }
}

/**
init keywords map;
*/
void Lexer::MapInit()
{
    symTypeMap["+"] = PLUS;
    symTypeMap["-"] = MINUS;
    symTypeMap["*"] = TIMES;
    symTypeMap["/"] = DIVIDE;

    symTypeMap["=="] = EQ;
    symTypeMap["<="] = LEQ;
    symTypeMap[">="] = GEQ;
    symTypeMap[">"] = GRT;
    symTypeMap["<"] = LES;
    symTypeMap["!="] = NEQ;

    symTypeMap["("] = Lpar;
    symTypeMap[")"] = Rpar;
    symTypeMap["["] = Lspar;
    symTypeMap["]"] = Rspar;
    symTypeMap["{"] = Lbrac;
    symTypeMap["}"] = Rbrac;

    symTypeMap[","] = COMMA;
    symTypeMap[";"] = SEMICOLON;
    symTypeMap[":"] = COLON;
    symTypeMap["="] = GIVEN;

    symTypeMap["const"] = symconst;
    symTypeMap["int"] = symint;
    symTypeMap["char"] = symchar;
    symTypeMap["void"] = symvoid;
    symTypeMap["main"] = symmain;
    symTypeMap["if"] = symif;
    symTypeMap["else"] = symelse;
    symTypeMap["for"] = symfor;
    symTypeMap["switch"] = symswitch;
    symTypeMap["case"] = symcase;
    symTypeMap["default"] = symdefault;
    symTypeMap["printf"] = symprintf;
    symTypeMap["scanf"] = symscanf;
    symTypeMap["return"] = symreturn;
}






