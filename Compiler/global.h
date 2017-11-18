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
struct midInstr {
    OpCode op;
    TableItem* dst;
    TableItem* src1;
    TableItem* src2;
};
#endif // GLOBAL_H_INCLUDED
