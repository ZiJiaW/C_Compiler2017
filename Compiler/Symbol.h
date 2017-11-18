#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <iostream>
#include <string>
#include "global.h"
using namespace std;
string GetTypeName(SymType t);
class Symbol{
public:
    Symbol(SymType type, int lineNum = -1, string name = "", int numVal = -1);
    Symbol();
    SymType type();
    string name();
    int numVal();
    int lineNum();
private:
    SymType _type; // 标识符的种类
    string _name; // 标识符的字符串内容，可以是字符'a'，字符串"abc"，普通定义的符号sum，数字123的字符串形式
    int _numVal; // 如果是数字的话，它的值
    int _lineNum; // 该标识符所在行号
};

#endif // SYMBOL_H_INCLUDED
