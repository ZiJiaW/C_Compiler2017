#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "Lexer.h"
#include "MiddleCode.h"
#include "SymbolTable.h"
#include "global.h"
class Parser {
public:
    Parser(Lexer &lex, ErrorHandler &eh, MiddleCode &mc, SymbolTable &rt);
    void StartParsing();     // 分析入口
private:
    vector<Symbol> tokens;   // 保存所有获取的单词
    vector<Symbol>::iterator curToken; // 当前单词迭代器
    void NextToken();        // 获取下一个单词，对GetSym进一步封装
    void BackToken(int n);   // 回退n个单词，用于预读判断

    Lexer &lex;              // 词法分析器调用
    ErrorHandler &eh;        // 错误处理器调用
    MiddleCode &mc;          // 中间代码表调用
    SymbolTable &rootTable;   // 根符号表
    TableItem *curItem;      // 当前符号表项指针
    SymbolTable *curTbl;     // 当前符号表指针

    void Program();          // 程序入口
    void ConstState();       // 常量说明
    void VarState();         // 变量说明
    void FuncWithRet();      // 有返回值函数定义
    void FuncWithoutRet();   // 无返回值函数定义
    void MainFunc();         // 主函数入口
    void ParaList();         // 函数定义参数表
    void ComplexSentence();  // 复合语句

    void SentenceList();     // 语句列
    void Sentence();         // 语句

    void GiveState();        // 赋值语句

    void IfState();          // 条件语句
    void Condition();        // 条件

    void ForState();         // 循环语句

    void SwitchState();      // 情况语句
    void CaseList();         // 情况表
    void CaseState();        // 情况子语句
    void DefaultState();     // 默认情况语句

    void CallFuncState();    // 函数调用语句
    void PrintState();       // 写语句
    void ScanfState();       // 读语句

    void Expression();       // 表达式
    void Term();             // 项
    void Factor();           // 因子
};

#endif // PARSER_H_INCLUDED
