#ifndef MIPSTRANSLATOR_H_INCLUDED
#define MIPSTRANSLATOR_H_INCLUDED
#include "MiddleCode.h"
#include "SymbolTable.h"
#include <queue>
//#define mipsDEBUG
struct RegRecord {
    Reg rg;
    vector<Reg> funcUsed;
    bool isGlobal;
    int offsetByGp; // if array it's base address
    int offsetByFp; // if array it's base address
    int stackSize;
    RegRecord(Reg _rg = NONE, bool ig = false, int og = 0, int of = 0, int ss = 0):
    rg(_rg), isGlobal(ig), offsetByGp(og), offsetByFp(of), stackSize(ss){}
};
class MIPSTranslator {
public:
    MIPSTranslator(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl, TableItem* Zero);
    void translate();
    vector<string> codes;
private:
    MiddleCode &mc;
    SymbolTable &tbl;
    vector<TableItem*> &tmptbl;
    TableItem* constZero;
    vector<TableItem*> ConstStrs;
    map<TableItem*, RegRecord*> RegMap;
    void Initialize();

    inline bool IsFunc(TableItem* t);
    inline bool IsArray(TableItem* t);
    inline bool IsVar(TableItem* t);
    inline bool IsConst(TableItem* t);
    string GetRegName(Reg rg);
    string itos(int n);
    string address(TableItem *t);

    TableItem* TempRegs[10];
    Reg TempAlloc(TableItem* t);
    queue<int> usedRegs;
    void TempRefresh(bool tostore = true);
    void Refresh(Reg rg);
};

#endif // MIPSTRANSLATOR_H_INCLUDED


/**思路：
 * 先扫描符号表和临时变量表，为全局变量和字符串分配地址空间于静态数据区$gp的上下区间，记录全局变量相对gp的偏移
 * 扫描到函数时，统计其内部所有用到的参数和变量（局部和临时），计算其栈空间
 * 并记录每一个内部变量的对应于fp的偏移
 * 运行栈结构:
 * *****************************************栈顶
 *             参数1                       *
 *             参数2                       *由调用者进行压栈操作
 *             参数3......                 *
 * *****************************************
 *              $fp
 *              $ra
 * *****************************************
 *             全局寄存器
 *
 * ******************************************
 *
 *
 *            局部和临时变量
 *
 *
 * ******************************************
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
*/
