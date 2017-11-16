#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "Symbol.h"
#include "ErrorHandler.h"

class Lexer{
public:
    Symbol GetSym();
    Lexer(string fileName, ErrorHandler &_errorHandler);
private:
    map<string, SymType> symTypeMap;
    void MapInit();

    string buffer;
    int lineNum;

    char curChar;
    int curIndex;

    fstream file;
    bool fileEmpty;

    bool NewLine();
    void NewChar();
    void Skip();

    ErrorHandler &errorHandler;
};

#endif // LEXER_H_INCLUDED
