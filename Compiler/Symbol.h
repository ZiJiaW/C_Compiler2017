#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <iostream>
#include <string>
using namespace std;
enum SymType {
    noone, // belong to nothing

    ident, // ��ʶ��
    constch, // �ַ� 'a'
    conststr, // �ַ��� "hello world"
    num, // �޷������� 512

    /*�����*/
    add, // +
    sub, // -
    times, // *
    divide, // '/'

    /*��ϵ�����*/
    eq, // ==
    leq, // <=
    geq, // >=
    grt, // >
    les, // <
    neq, // !=

    /*����*/
    lpar, // '('
    rpar, // ')'
    lspar, // '['
    rspar, // ']'
    lbrac, // '{'
    rbrac, // '}'

    /*�����ַ�*/
    quo, // ' ������
    dquo, // " ˫����
    comma, // , ����
    semicolon, // ; �ֺ�
    colon, // : ð��
    given, // = ��ֵ

    /*�ؼ���*/
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
    SymType type; // ��ʶ��������
    string name; // ��ʶ�����ַ������ݣ��������ַ�'a'���ַ���"abc"����ͨ����ķ���sum������123���ַ�����ʽ
    int numVal; // ��������ֵĻ�������ֵ
    int lineNum; // �ñ�ʶ�������к�
};

#endif // SYMBOL_H_INCLUDED
