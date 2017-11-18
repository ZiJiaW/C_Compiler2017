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
    SymType _type; // ��ʶ��������
    string _name; // ��ʶ�����ַ������ݣ��������ַ�'a'���ַ���"abc"����ͨ����ķ���sum������123���ַ�����ʽ
    int _numVal; // ��������ֵĻ�������ֵ
    int _lineNum; // �ñ�ʶ�������к�
};

#endif // SYMBOL_H_INCLUDED
