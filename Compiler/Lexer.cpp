#include <iostream>
#include <string>
#include <fstream>
#include "Symbol.h"
#include "Lexer.h"
/**
1. open source file;
2. get first line and set lineNum to 1;
3. set current index to 0;
*/
Lexer::Lexer(string fileName)
{
    file.open(fileName.c_str(), ios::in);
    getline(file, buffer);
    lineNum = 1;
    curch = 0;
}
