#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
enum OpCode {
    // �м�������ݽṹ�д洢���ű�ָ��
    // ��˿��Ի�ñ�����������Ϣ
    // ADD t a b(t = a + b)
    ADD = 100, SUB, MUL, DIV,
    GIV, // GIV a b(a = b)
    STR, // STR a(int a/char a/int a[10]/char a[10])
    SETL, // SETL Label(��ǩ��Ҳ��Ҫ������ű�ɣ�����)
    PARA, // PARA a(������������)
    PUSH, // PUSH a(����������ջ)
    CALL, // CALL FUNC(���ú���FUNC)
    JMP, // JMP LABEL(��������ת)
    NEG, // NEG a b(a=-b)
    RET, // RET a(return a)
    ARR_GIV, // []= a k n    a[k]=n
    GIV_ARR, // =[] n a k    n=a[k]
    BGR, // branch to label if src1 is greater than src2 : BGR LABEL src1 src2
    BEQ, // branch if equal
    BLS, // branch if less
    BNE, // branch if not equal
    BGE, // branch if greater or equal
    BLE, // branch if less or equal
    WRITE, // WRITE t(printf(t) or printf("string")) const string should be inserted into Symbol table too!
    READ   // READ t(scanf(t))
};
enum TableItemType {
    CONST_INT = 150, CONST_CHAR, INT, CHAR, VOID_FUNC, INT_FUNC, CHAR_FUNC, INT_ARR, CHAR_ARR
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
#endif // GLOBAL_H_INCLUDED
