#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#include "TableItem.h"
enum OpCode {
    ADD, SUB, MUL, DIV,
    GIV, STR, SETL, PARA, PUSH, CALL,
    JMP, NEG, RET,
    ARR_GIV, // []= a k n    a[k]=n
    GIV_ARR, // =[] n a k    n=a[k]
    BGR, // branch to label if src1 is greater than src2 : BGR LABEL src1 src2
    BEQ, // branch if equal
    BLS, // branch if less
    BNE, // branch if not equal
    BGE, // branch if greater or equal
    BLE, // branch if less or equal
    WRITE, READ
};
enum TableItemType {
    CONST_INT, CONST_CHAR, INT, CHAR, VOID_FUNC, INT_FUNC, CHAR_FUNC, INT_ARR, CHAR_ARR
};
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
struct midInstr {
    OpCode op;
    TableItem* dst;
    TableItem* src1;
    TableItem* src2;
};
#endif // GLOBAL_H_INCLUDED
