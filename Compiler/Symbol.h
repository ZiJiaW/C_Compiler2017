#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <iostream>
#include <string>
using namespace std;
enum SymType {
    noone, // belong to nothing

    ident, // 标识符
    constch, // 字符 'a'
    conststr, // 字符串 "hello world"
    num, // 无符号整数 512

    /*运算符*/
    add, // +
    sub, // -
    times, // *
    divide, // '/'

    /*关系运算符*/
    eq, // ==
    leq, // <=
    geq, // >=
    grt, // >
    les, // <
    neq, // !=

    /*括号*/
    lpar, // '('
    rpar, // ')'
    lspar, // '['
    rspar, // ']'
    lbrac, // '{'
    rbrac, // '}'

    /*其他字符*/
    quo, // ' 单引号
    dquo, // " 双引号
    comma, // , 逗号
    semicolon, // ; 分号
    colon, // : 冒号
    given, // = 赋值

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
    Symbol(string _name, SymType _type, int _numVal, int _lineNum);
    SymType type();
    string name();
    int numVal();
    int lineNum();
private:
    SymType type; // 标识符的种类
    string name; // 标识符的字符串内容，可以是字符'a'，字符串"abc"，普通定义的符号sum，数字123的字符串形式
    int numVal; // 如果是数字的话，它的值
    int lineNum; // 该标识符所在行号
};

#endif // SYMBOL_H_INCLUDED
