#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "Lexer.h"
#include "MiddleCode.h"
#include "SymbolTable.h"
#include "global.h"
class Parser {
public:
    Parser(Lexer &lex, ErrorHandler &eh, MiddleCode &mc, SymbolTable &rt);
    void StartParsing();     // �������
private:
    vector<Symbol> tokens;   // �������л�ȡ�ĵ���
    vector<Symbol>::iterator curToken; // ��ǰ���ʵ�����
    void NextToken();        // ��ȡ��һ�����ʣ���GetSym��һ����װ
    void BackToken(int n);   // ����n�����ʣ�����Ԥ���ж�

    Lexer &lex;              // �ʷ�����������
    ErrorHandler &eh;        // ������������
    MiddleCode &mc;          // �м��������
    SymbolTable &rootTable;   // �����ű�
    TableItem *curItem;      // ��ǰ���ű���ָ��
    SymbolTable *curTbl;     // ��ǰ���ű�ָ��

    void Program();          // �������
    void ConstState();       // ����˵��
    void VarState();         // ����˵��
    void FuncWithRet();      // �з���ֵ��������
    void FuncWithoutRet();   // �޷���ֵ��������
    void MainFunc();         // ���������
    void ParaList();         // �������������
    void ComplexSentence();  // �������

    void SentenceList();     // �����
    void Sentence();         // ���

    void GiveState();        // ��ֵ���

    void IfState();          // �������
    void Condition();        // ����

    void ForState();         // ѭ�����

    void SwitchState();      // ������
    void CaseList();         // �����
    void CaseState();        // ��������
    void DefaultState();     // Ĭ��������

    void CallFuncState();    // �����������
    void PrintState();       // д���
    void ScanfState();       // �����

    void Expression();       // ���ʽ
    void Term();             // ��
    void Factor();           // ����
};

#endif // PARSER_H_INCLUDED
