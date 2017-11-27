#include <algorithm>
#include "MIPSTranslator.h"

MIPSTranslator::MIPSTranslator(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl):
mc(_mc), tbl(_tbl), tmptbl(_tmptbl){}

bool MIPSTranslator::IsFunc(TableItem* t)
{
    return t->type()==VOID_FUNC || t->type()==INT_FUNC || t->type()==CHAR_FUNC;
}
bool MIPSTranslator::IsArray(TableItem *t)
{
    return t->type()==INT_ARR || t->type()==CHAR_ARR;
}
bool MIPSTranslator::IsVar(TableItem* t)
{
    return t->type()==INT || t->type()==CHAR;
}
Reg MIPSTranslator::GetRegBySx(int i)
{
    return Reg(s0 + i);
}
/**
 * scan SymbolTable and TempTable to get info;
 * allocate global regs to local variables by counting;
 * give an offset by sp/gp to local/global variables;
 * calculate stack size of functions;
 * all by initialize RegMap;
*/
void MIPSTranslator::Initialize()
{
    int offgp = 0;
    for(map<string, TableItem*>::iterator p = tbl.table.begin(); p != tbl.table.end(); p++)
    {
        TableItem* cur = p->second;
        if(IsVar(cur))// 全局变量，进行地址分配
        {
            RegMap[cur] = new RegRecord(NONE, true, offgp);
            offgp += 4;
        }
        else if(IsArray(cur))//全局数组
        {
            RegMap[cur] = new RegRecord(NONE, true, offgp);
            offgp += 4*cur->length();
        }
        else if(IsFunc(cur))//函数
        {
            RegRecord *rcd = new RegRecord(NONE, true);
            RegMap[cur] = rcd;
            if(cur->name() != "main")
            {
                rcd->stackSize += 4; // 保存ra
            }
            // 遍历函数子符号表的局部变量，分配地址，计算栈空间
            // 同时通过引用计数进行全局寄存器分配
            int offsp = 0; // 地址相对栈偏移
            TableItem* Alloc[8]; // s0-s7的分配数组
            for(int i = 0; i < 8; i++)
                Alloc[i] = NULL;
            for(map<string, TableItem*>::iterator pp = cur->funcField()->table.begin();
                pp != cur->funcField()->table.end(); pp++)
            {
                TableItem* cc = pp->second;
                // 如果cc是参数，其位置在栈底（因为是在函数调用以前进行PUSH的）
                // 等计算完栈空间才能分配地址
                vector<string>::iterator it = find(cur->paraName.begin(), cur->paraName.end(), cc->name());
                if(it != cur->paraName.end())
                {
                    rcd->stackSize += 4;
                    RegMap[cc] = new RegRecord();
                    continue;
                }
                if(IsVar(cc))// 局部变量，要进行全局寄存器分配
                {
                    RegRecord* child = new RegRecord();
                    child->offsetBySp = offsp;
                    offsp += 4;
                    RegMap[cc] = child;
                    rcd->stackSize += 4; // 函数栈空间增加
                    // 尝试为cc分配寄存器，遍历分配数组找空闲寄存器
                    bool finded = false;
                    for(int i = 0; i < 8; i++)
                    {
                        if(Alloc[i] == NULL)
                        {
                            finded = true;
                            Alloc[i] = cc;
                            child->rg = Reg(s0 + i);
                            break;
                        }
                    }
                    if(!finded) // 如果没找到，尝试踢掉一个引用次数比它少的
                    {
                        for(int i = 0; i < 8; i++)
                        {
                            if(Alloc[i]->useCount < cc->useCount)
                            {
                                RegMap[Alloc[i]]->rg = NONE;
                                Alloc[i] = cc;
                                child->rg = Reg(s0 + i);
                                break;
                            }
                        }// 再找不到就不管了
                    }
                }
                else if(IsArray(cc))// 局部数组
                {
                    RegRecord* child = new RegRecord();
                    rcd->stackSize += 4 * cc->length(); // 分配数组空间
                    child->offsetBySp = offsp;
                    offsp += 4 * cc->length(); // 数组空间单独占用
                }
            }
            // 遍历函数内部的临时变量表，为其中TMP类型分配栈空间和地址
            for(vector<TableItem*>::iterator it = cur->funcField()->temp.begin();
                it != cur->funcField()->temp.end(); it++)
            {
                if((*it)->type() == TMP)
                {
                    rcd->stackSize += 4;
                    RegMap[*it] = new RegRecord(NONE, false, 0, offsp);
                    offsp += 4;
                }
            }
            // 现在的栈空间包含了ra,所有需要保存的临时变量和局部变量
            // 下面计算需要保存的全局寄存器，即函数中用到的全局寄存器
            for(int i = 0; i < 8; i++)
            {
                if(Alloc[i])
                {
                    rcd->stackSize += 4; // 保存全局寄存器需要的空间
                    rcd->funcUsed.push_back(Reg(s0+i)); // 记录用到的全局寄存器
                }
            }
            // 现在为函数的参数分配地址
            for(vector<string>::iterator it = cur->paraName.begin();
                it != cur->paraName.end(); it++)
            {
                TableItem* para = cur->funcField()->GetItem(*it);
                // 参数自左向右入栈，在栈顶
                RegMap[para]->offsetBySp = rcd->stackSize - 4 * (it - cur->paraName.begin() + 1);
            }
        }
    }
}
/**
 * scan MiddleCode to generate MIPS codes;
*/
void MIPSTranslator::translate()
{
    // 为字符串分配空间

    // 生成跳转到main的指令和程序退出指令

    // 逐条生成代码
    for(vector<midInstr>::iterator it = mc.code.begin(); it != mc.code.end(); it++)
    {
        switch((*it).op)
        {
            case ADD: {

            }
            case SUB: {

            }
            case MUL: {

            }
            case DIV: {

            }
            case GIV: {

            }
            case SETL: {

            }
            case FUNC: {

            }
            case PARA: {

            }
            case PUSH: {

            }
            case CALL: {

            }
            case JMP: {

            }
            case NEG: {

            }
            case RET: {

            }
            case GET_RET: {

            }
            case GIV_ARR: {

            }
            case ARR_GIV: {

            }
            case BEQ: {

            }
            case BNE: {

            }
            case BGEZ: {

            }
            case BGTZ: {

            }
            case BLEZ: {

            }
            case BLTZ: {

            }
            case WRITE: {

            }
            case READ: {

            }
            case END_FUNC: {

            }
            case END_PROC: {

            }
        }
    }
}