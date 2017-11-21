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
/**
 * 供外部调用的接口
 * 每次调用获取一个token
*/
    Symbol GetSym();
/**
 * 构造函数，传入源文件名，错误处理实例的引用
*/
    Lexer(string fileName, ErrorHandler &_errorHandler);
private:
/**
 * 关键字和token类型的map，用于匹配关键字
*/
    map<string, SymType> symTypeMap;
// 初始化上面的map，在构造函数中进行
    void MapInit();
// 一次读取一行内容进入buffer
    string buffer;
// 记录正在处理的行号
    int lineNum;
// 记录当前正在处理的字符
    char curChar;
// 记录当前正在处理的字符在buffer中的位置
    int curIndex;
// 源文件流
    fstream file;
    bool fileEmpty;
// 获取下一行
    bool NewLine();
// 获取下一个字符
    void NewChar();
// 跳读直到遇到空字符
    void Skip();
// 错误处理器的引用
    ErrorHandler &errorHandler;
};

#endif // LEXER_H_INCLUDED
