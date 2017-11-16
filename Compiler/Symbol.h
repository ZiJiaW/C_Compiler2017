#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <iostream>
#include <string>
using namespace std;
enum SymType {
    noone, // belong to nothing
    END, // end of code
    ERR, // got an illegal symbol

    IDENT, // 标识符
    constch, // 字符 'a'
    conststr, // 字符串 "hello world"
    NUM, // 无符号整数 512

    /*运算符*/
    PLUS, // +
    MINUS, // -
    TIMES, // *
    DIVIDE, // '/'

    /*关系运算符*/
    EQ, // ==
    LEQ, // <=
    GEQ, // >=
    GRT, // >
    LES, // <
    NEQ, // !=

    /*括号*/
    Lpar, // '('
    Rpar, // ')'
    Lspar, // '['
    Rspar, // ']'
    Lbrac, // '{'
    Rbrac, // '}'

    /*其他字符*/
    QUO, // ' 单引号
    DQUO, // " 双引号
    COMMA, // , 逗号
    SEMICOLON, // ; 分号
    COLON, // : 冒号
    GIVEN, // = 赋值

    /*关键字*/
    symconst,
    symint,
    symchar,
    symvoid,
    symmain,
    symif,
    symelse,
    symfor,
    symswitch,
    symcase,
    symdefault,
    symprintf,
    symscanf,
    symreturn
};

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
