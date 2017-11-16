#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED
#include <iostream>
#include <string>
using namespace std;
enum SymType {
    noone, // belong to nothing
    END, // end of code
    ERR, // got an illegal symbol

    IDENT, // ��ʶ��
    constch, // �ַ� 'a'
    conststr, // �ַ��� "hello world"
    NUM, // �޷������� 512

    /*�����*/
    PLUS, // +
    MINUS, // -
    TIMES, // *
    DIVIDE, // '/'

    /*��ϵ�����*/
    EQ, // ==
    LEQ, // <=
    GEQ, // >=
    GRT, // >
    LES, // <
    NEQ, // !=

    /*����*/
    Lpar, // '('
    Rpar, // ')'
    Lspar, // '['
    Rspar, // ']'
    Lbrac, // '{'
    Rbrac, // '}'

    /*�����ַ�*/
    QUO, // ' ������
    DQUO, // " ˫����
    COMMA, // , ����
    SEMICOLON, // ; �ֺ�
    COLON, // : ð��
    GIVEN, // = ��ֵ

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
