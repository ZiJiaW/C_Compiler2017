#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "Lexer.h"
#include "MiddleCode.h"
#include "SymbolTable.h"
#include "global.h"
class Parser {
public:
    Parser(Lexer &lex, ErrorHandler &eh, MiddleCode &mc, SymbolTable &rt);
    void StartParsing();     // start entry
    vector<TableItem*> TempTable;   // 临时变量表，用于存储表达式中生成的中间变量，和常量
private:
    int tempIndex;  // 当前临时变量在临时变量表中的位置

    TableItem* NewTmpVal();     // 在临时变量表中添加临时变量，并返回其表项指针
    TableItem* NewTmpVal(int constval); // 添加整数常量，返回表项指针
    TableItem* NewTmpVal(char constchar); // 添加字符常量，返回表项指针

    vector<Symbol> tokens;   // 保存所有单词
    vector<Symbol>::iterator curToken; // 指向当前正在处理的单词
    void NextToken();        // 获取下一个单词
    inline void BackToken(int n);   // 回退n个单词
    // Overloaded skip func
    void SkipUntil(SymType type); // 跳读
    void SkipUntil(SymType type1, SymType type2);
    bool SkipInlineUntil(SymType type); // 行内跳读，如果没找到跳读单元，跳至下一行首并返回false

    void SetError(int errnum){eh.SetError(curToken->lineNum(), errnum, curToken->name());}
    void SetError(int errnum, string name){eh.SetError(curToken->lineNum(), errnum, name);}

    Lexer &lex;              // 词法分析器
    ErrorHandler &eh;        // 错误处理器
    MiddleCode &mc;          // 中间代码表
    SymbolTable &rootTable;   // 根符号表
    TableItem *curItem;      // 当前符号表项指针
    TableItem *curFunc;      // 当前正在处理的函数的符号表项指针
    SymbolTable *curTbl;     // 当前符号表指针

    bool InsertTable(string name, TableItemType type, int value = 0);
    TableItem* GetItemByName(string name);
    string GetTempName();

    void Program();          // 程序入口1
    void ConstState();       // 常量说明1
    void VarState();         // 变量说明1
    void FuncWithRet();      // 有返回值函数1
    void FuncWithoutRet();   // 无返回值函数1
    void MainFunc();         // 主函数1
    void ParaList(TableItem* funcitem);// 参数表 需要传函数在根符号表的符号表项，用于记录该函数参数内容1
    void ComplexSentence();  // 复合语句1

    void SentenceList();     // 语句列1
    void Sentence();         // 语句1

    void GiveState();        // 赋值语句

    void IfState();          // 条件语句
    void Condition();        // 条件

    void ForState();         // 循环语句

    void SwitchState();      // 情况语句
    void CaseList();         // 情况表
    void CaseState();        // 情况子语句
    void DefaultState();     // 默认

    TableItem* CallFuncState();    // 函数调用语句
    void PrintState();       // 打印
    void ScanfState();       // 读
    void ReturnState();      // 返回语句
    // 表达式生成的临时变量同样加入临时变量表
    TableItem* Expression();       // 表达式1
    TableItem* Term();             // 项1
    TableItem* Factor();           // 因子1
};

#endif // PARSER_H_INCLUDED
