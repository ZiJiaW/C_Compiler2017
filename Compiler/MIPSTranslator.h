#ifndef MIPSTRANSLATOR_H_INCLUDED
#define MIPSTRANSLATOR_H_INCLUDED
#include "MiddleCode.h"
#include "SymbolTable.h"
struct RegRecord {
    Reg rg;
    vector<Reg> funcUsed;
    bool isGlobal;
    int offsetByGp; // if array it's base address
    int offsetBySp; // if array it's base address
    int stackSize;
    RegRecord(Reg _rg = NONE, bool ig = false, int og = 0, int os = 0, int ss = 0):
    rg(_rg), isGlobal(ig), offsetByGp(og), offsetBySp(os), stackSize(ss){}
};
class MIPSTranslator {
public:
    MIPSTranslator(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl);
    void translate();
    vector<string> codes;
private:
    MiddleCode &mc;
    SymbolTable &tbl;
    vector<TableItem*> &tmptbl;


    vector<TableItem*> GlobalVar;
    vector<TableItem*> ConstStrs;
    map<TableItem*, RegRecord*> RegMap;
    void Initialize();

    inline bool IsFunc(TableItem* t);
    inline bool IsArray(TableItem* t);
    inline bool IsVar(TableItem* t);

    Reg GetRegBySx(int i);
};

#endif // MIPSTRANSLATOR_H_INCLUDED


/**思路：
 * 先扫描符号表和临时变量表，为全局变量和字符串分配地址空间于静态数据区$gp的上下区间，记录全局变量相对gp的偏移
 * 扫描到函数时，统计其内部所有用到的参数和变量（局部和临时），计算其栈空间
 * 并记录每一个内部变量的对应于sp的偏移
 * 
 * 排序函数内部局部变量在中间代码中的引用次数，分配全局寄存器，并记录到该函数对应的数据结构中
 * 
*/
