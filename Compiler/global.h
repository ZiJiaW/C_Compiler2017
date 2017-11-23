#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
enum OpCode {
    // 中间代码数据结构中存储符号表指针
    // 因此可以获得变量的所有信息
    // ADD t a b(t = a + b)
    ADD = 100, SUB, MUL, DIV,
    GIV, // GIV a b(a = b)
    STR, // STR a(int a/char a/int a[10]/char a[10])
    SETL, // SETL Label(标签名也需要加入符号表吧？？？)
    FUNC, // 函数声明
    PARA, // PARA a(函数参数定义)
    PUSH, // PUSH a(函数参数入栈)
    CALL, // CALL FUNC(调用函数FUNC)
    JMP, // JMP LABEL(无条件跳转)
    NEG, // NEG a b(a=-b)
    RET, // RET a(return a)
    GET_RET, // a = return val
    GIV_ARR, // []= a k n    a[k]=n
    ARR_GIV, // =[] n a k    n=a[k]
    BGR, // branch to label if src1 is greater than src2 : BGR LABEL src1 src2
    BEQ, // branch if equal
    BLS, // branch if less
    BNE, // branch if not equal
    BGE, // branch if greater or equal
    BLE, // branch if less or equal
    WRITE, // WRITE t(printf(t) or printf("string")) const string should be inserted into Symbol table too!
    READ,   // READ t(scanf(t))
    END_FUNC, // 函数结束
    END_PROC, // 程序结束
};
enum TableItemType {
    CONST_INT = 150, CONST_CHAR, INT, CHAR, VOID_FUNC, INT_FUNC, CHAR_FUNC, INT_ARR, CHAR_ARR, TMP, CONST_STR
};
enum SymType {
    noone, // belong to nothing
    END, // end of code
    ERR, // got an illegal symbol

    IDENT, // 标识符
    constch, // 字符'a'
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
#endif // GLOBAL_H_INCLUDED
