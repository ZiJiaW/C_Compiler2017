#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include "Symbol.h"

class Lexer{
public:
    Symbol GetSym();
    Lexer(string fileName);
private:
    string buffer;
    int curch;
    int lineNum;
    bool NewLine();
    fstream file;
};

#endif // LEXER_H_INCLUDED
