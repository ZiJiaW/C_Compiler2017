#include <algorithm>
#include <cstdlib>
#include "MIPSTranslator.h"

MIPSTranslator::MIPSTranslator(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl):
mc(_mc), tbl(_tbl), tmptbl(_tmptbl){
    for(int i = 0; i<10; i++)
        TempRegs[i] = NULL;
}

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
bool MIPSTranslator::IsConst(TableItem* t)
{
    return t->type()==CONST_INT || t->type()==CONST_CHAR;
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
        #ifdef mipsDEBUG
            cout<<"test: "<<offgp<<endl;
        #endif // mipsDEBUG
        TableItem* cur = p->second;
        if(IsVar(cur))// 全局变量，进行地址分配
        {
            RegMap[cur] = new RegRecord(NONE, true, offgp);
            offgp += 4;
            #ifdef mipsDEBUG
            cout<<"Global var:"<<cur->name()<<endl;
            cout<<RegMap[cur]->isGlobal<<"      "<<RegMap[cur]->offsetByGp<<endl;
            #endif // mipsDEBUG
        }
        else if(IsArray(cur))//全局数组
        {
            RegMap[cur] = new RegRecord(NONE, true, offgp);
            offgp += 4*cur->length();
            #ifdef mipsDEBUG
            cout<<"Global array:"<<cur->name()<<"length: "<<cur->length()<<endl;
            cout<<RegMap[cur]->isGlobal<<"     "<<RegMap[cur]->offsetByGp<<endl;
            #endif // mipsDEBUG
        }
        else if(IsConst(cur))
        {
            RegMap[cur] = new RegRecord();// const常量不分配地址，生成代码的时候可能会分配寄存器用于计算
        }
        else if(IsFunc(cur))//函数
        {
            RegRecord *rcd = new RegRecord(NONE, true);
            RegMap[cur] = rcd;
            if(cur->name() != "main")
            {
                rcd->stackSize += 8; // 保存$ra & $fp
            }
            else
                rcd->stackSize += 4;
            // 遍历函数子符号表的局部变量，分配地址，计算栈空间
            // 同时通过引用计数进行全局寄存器分配
            int offfp = 0; // 地址相对fp偏移
            TableItem* Alloc[8]; // s0-s7的分配数组
            for(int i = 0; i < 8; i++)
                Alloc[i] = NULL;
            for(map<string, TableItem*>::iterator pp = cur->funcField()->table.begin();
                pp != cur->funcField()->table.end(); pp++)
            {
                TableItem* cc = pp->second;
                // 如果cc是参数，其位置在栈顶（因为是在函数调用以前进行PUSH的）
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
                    child->offsetByFp = offfp;
                    offfp += 4;
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
                    #ifdef mipsDEBUG
                    cout<<"Local array: "<<cc->name()<<endl;
                    #endif // mipsDEBUG
                    RegRecord* child = new RegRecord();
                    rcd->stackSize += 4 * cc->length(); // 分配数组空间
                    child->offsetByFp = offfp;
                    RegMap[cc] = child;
                    offfp += 4 * cc->length(); // 数组空间单独占用
                }
                else if(IsConst(cc))
                {
                    RegMap[cc] = new RegRecord();
                }
            }
            // 遍历函数内部的临时变量表，为其中TMP类型分配栈空间和地址，同时记录字符串信息
            for(vector<TableItem*>::iterator it = cur->funcField()->temp.begin();
                it != cur->funcField()->temp.end(); it++)
            {
                if((*it)->type() == TMP)
                {
                    rcd->stackSize += 4;
                    RegMap[*it] = new RegRecord(NONE, false, 0, offfp);
                    offfp += 4;
                }
                else if((*it)->type() == CONST_STR)
                {
                    ConstStrs.push_back(*it);
                }
                else if(IsConst(*it))
                {
                    RegMap[*it] = new RegRecord();
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
                RegMap[para]->offsetByFp = rcd->stackSize - 4 * (it - cur->paraName.begin() + 1);
            }
        }
    }
}
/**
 * get register name by enum var Reg;
*/
string MIPSTranslator::GetRegName(Reg rg)
{
    if(rg >= t0 && rg <= t9)
    {
        string ret("$t");
        ret.append(1, char('0'+rg-t0));
        return ret;
    }
    if(rg >= s0 && rg <= s7)
    {
        string ret("$s");
        ret.append(1, char('0'+rg-s0));
        return ret;
    }
}
/**
 * transform integer to string;
*/
string MIPSTranslator::itos(int n)
{
    char buffer[20];
    itoa(n, buffer, 10);
    return string(buffer);
}
/**
 * allocate temp register to the given table item
 * return the reg allocated to it
 * and do the store work if someone's kicked
*/
Reg MIPSTranslator::TempAlloc(TableItem* t)
{
    for(int i = 0; i < 10; i++)
    {
        if(TempRegs[i] == NULL)
        {
            TempRegs[i] = t;
            RegMap[t]->rg = Reg(t0 + i);
            usedRegs.push(i);
            return Reg(t0 + i);
        }
    }
    // fail to find a reg, choose a longest-used reg
    int regIndex = usedRegs.front();
//    if(!IsConst(t))//常数的寄存器使用保存优先级较低，不进行队列调整
    {
        usedRegs.pop();
        usedRegs.push(regIndex);
    }
    TableItem* tokick = TempRegs[regIndex];
    if(!IsConst(tokick))// const常量不分配地址，因此不需要保存
    {
        codes.push_back(string("sw ")+ GetRegName(Reg(t0+regIndex)) +", " + address(tokick));
    }
    RegMap[t]->rg = RegMap[tokick]->rg;
    RegMap[tokick]->rg = NONE;
    TempRegs[regIndex] = t;
    return Reg(t0 + regIndex);
}
/**
 * refresh all temp registers and store their values into RAM;
 * It's called when stepping into a new block;
*/
void MIPSTranslator::TempRefresh(bool tostore)
{
    for(int i = 0; i < 10; i++)
    {
        if(TempRegs[i] != NULL)
        {
            if(!IsConst(TempRegs[i]) && tostore)
                codes.push_back(string("sw ")+ GetRegName(Reg(t0+i)) +", " + address(TempRegs[i]));
            RegMap[TempRegs[i]]->rg = NONE;
            TempRegs[i] = NULL;
        }
    }
    usedRegs = queue<int>();
}
/**
 * refresh one register after pushing sth into next func's stack;
*/
void MIPSTranslator::Refresh(Reg rg)
{
    if(rg >= t0 && rg <= t9)
    {
        RegMap[TempRegs[rg - t0]]->rg = NONE;
        TempRegs[rg - t0] = NULL;
    }
}
/**
 * scan MiddleCode to generate MIPS codes;
*/
void MIPSTranslator::translate()
{
    Initialize();
    // 为字符串分配空间
    codes.push_back(".data");
    for(vector<TableItem*>::iterator it = ConstStrs.begin(); it != ConstStrs.end(); it++)
    {
        codes.push_back((*it)->name()+": .asciiz \""+(*it)->paraName[0]+"\"");
    }
    // 生成跳转到main的指令和程序退出指令
    codes.push_back(".text");
    codes.push_back("jal main");
    codes.push_back("li $v0, 10");
    codes.push_back("syscall");
    // 逐条生成代码
    for(vector<midInstr>::iterator it = mc.code.begin(); it != mc.code.end(); it++)
    {
        midInstr t = *it;
        #ifdef mipsDEBUG
        cout<<GetOpString(t.op)<<endl;
        #endif // mipsDEBUG
        switch(t.op)
        {
            case ADD: {
                if(IsConst(t.src1) && IsConst(t.src2))// t=2+3
                {
                    int tmp = t.src1->value() + t.src2->value();
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    // li $xx, 5
                    codes.push_back(string("li ")+GetRegName(RegMap[t.dst]->rg)+", "+itos(tmp));
                }
                else if(IsConst(t.src1) || IsConst(t.src2))// addi $xx, $xx, 100
                {
                    int val = IsConst(t.src1) ? t.src1->value() : t.src2->value();
                    TableItem* tsrc = IsConst(t.src1) ? t.src2 : t.src1;
                    if(RegMap[tsrc]->rg == NONE)
                    {
                        Reg r = TempAlloc(tsrc);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(tsrc));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("addi " + GetRegName(RegMap[t.dst]->rg) +
                            ", " + GetRegName(RegMap[tsrc]->rg) + ", " + itos(val)));
                }
                else// add $xx, $xx, $xx
                {
                    // 如果没有的话，为两个源操作数分配临时寄存器，并从内存中取值
                    if(RegMap[t.src1]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src1);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src1));
                    }
                    if(RegMap[t.src2]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src2);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src2));
                    }
                    // 为目标操作数分配寄存器（因为直接写入，所以不需要lw）
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("add ")+GetRegName(RegMap[t.dst]->rg)+", "+
                                GetRegName(RegMap[t.src1]->rg)+", "+GetRegName(RegMap[t.src2]->rg));
                }
                break;
            }
            case SUB: {
                if(IsConst(t.src1) && IsConst(t.src2))// t=2-3
                {
                    int tmp = t.src1->value() - t.src2->value();
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    // li $xx, 5
                    codes.push_back(string("li ")+GetRegName(RegMap[t.dst]->rg)+", "+itos(tmp));
                }
                else if(IsConst(t.src2))// t = a-2
                {
                    int val = t.src2->value();
                    TableItem* tsrc = t.src1;
                    if(RegMap[tsrc]->rg == NONE)
                    {
                        Reg r = TempAlloc(tsrc);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(tsrc));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("subi " + GetRegName(RegMap[t.dst]->rg) + ", " + GetRegName(RegMap[tsrc]->rg) + ", " + itos(val)));
                }
                else// sub $xx, $xx, $xx  t = 2 - a or b - a
                {
                    if(RegMap[t.src1]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src1);
                        if(RegMap[t.src1]->isGlobal)
                            codes.push_back(string("lw ") + GetRegName(r) + ", " + itos(RegMap[t.src1]->offsetByGp) + "($gp)");
                        else{
                            if(IsConst(t.src1))
                                codes.push_back(string("li ") + GetRegName(r)+", "+itos(t.src1->value())); // li $t0, 2
                            else
                                codes.push_back(string("lw ") + GetRegName(r) + ", " + itos(RegMap[t.src1]->offsetByFp) + "($fp)");
                        }
                    }
                    if(RegMap[t.src2]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src2);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src2));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("sub ")+GetRegName(RegMap[t.dst]->rg)+", "+
                                GetRegName(RegMap[t.src1]->rg)+", "+GetRegName(RegMap[t.src2]->rg));
                }
                break;
            }
            case MUL: {
                if(IsConst(t.src1) && IsConst(t.src2))// t=2*3
                {
                    int tmp = t.src1->value() * t.src2->value();
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    // li $xx, 6
                    codes.push_back(string("li ")+GetRegName(RegMap[t.dst]->rg)+", "+itos(tmp));
                }
                else if(IsConst(t.src1) || IsConst(t.src2))
                {
                    int val = IsConst(t.src1) ? t.src1->value() : t.src2->value();
                    TableItem* tsrc = IsConst(t.src1) ? t.src2 : t.src1;
                    if(RegMap[tsrc]->rg == NONE)
                    {
                        Reg r = TempAlloc(tsrc);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(tsrc));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("mulo " + GetRegName(RegMap[t.dst]->rg) +
                            ", " + GetRegName(RegMap[tsrc]->rg) + ", " + itos(val)));
                }
                else
                {
                    if(RegMap[t.src1]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src1);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src1));
                    }
                    if(RegMap[t.src2]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src2);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src2));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("mulo ")+GetRegName(RegMap[t.dst]->rg)+", "+
                            GetRegName(RegMap[t.src1]->rg)+", "+GetRegName(RegMap[t.src2]->rg));
                }
                break;
            }
            case DIV: {
                if(IsConst(t.src1) && IsConst(t.src2))// t=2/3
                {
                    int tmp = t.src1->value() / t.src2->value();
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    // li $xx, 0
                    codes.push_back(string("li ")+GetRegName(RegMap[t.dst]->rg)+", "+itos(tmp));
                }
                else if(IsConst(t.src2))// t = a/2
                {
                    int val = t.src2->value();
                    TableItem* tsrc = t.src1;
                    if(RegMap[tsrc]->rg == NONE)
                    {
                        Reg r = TempAlloc(tsrc);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(tsrc));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("div " + GetRegName(RegMap[t.dst]->rg) + ", " +
                                GetRegName(RegMap[tsrc]->rg) + ", " + itos(val)));
                }
                else// t = 2 / a or b / a
                {
                    if(RegMap[t.src1]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src1);
                        if(RegMap[t.src1]->isGlobal)
                            codes.push_back(string("lw ") + GetRegName(r) + ", " + itos(RegMap[t.src1]->offsetByGp) + "($gp)");
                        else{
                            if(IsConst(t.src1))
                                codes.push_back(string("li ") + GetRegName(r)+", "+itos(t.src1->value())); // li $t0, 2
                            else
                                codes.push_back(string("lw ") + GetRegName(r) + ", " + itos(RegMap[t.src1]->offsetByFp) + "($fp)");
                        }
                    }
                    if(RegMap[t.src2]->rg == NONE)
                    {
                        Reg r = TempAlloc(t.src2);
                        codes.push_back(string("lw ") + GetRegName(r) + ", " + address(t.src2));
                    }
                    if(RegMap[t.dst]->rg == NONE)
                        TempAlloc(t.dst);
                    codes.push_back(string("div ")+GetRegName(RegMap[t.dst]->rg)+", "+
                        GetRegName(RegMap[t.src1]->rg)+", "+GetRegName(RegMap[t.src2]->rg));
                }
                break;
            }
            case GIV: {
                if(RegMap[t.dst]->rg == NONE)
                    TempAlloc(t.dst);
                if(IsConst(t.src1))// li $t0, 5   t = 5
                    codes.push_back(string("li ")+GetRegName(RegMap[t.dst]->rg)+", "+itos(t.src1->value()));
                else // move $t0, $t1 or lw $t0, xx($xp)
                {
                    if(RegMap[t.src1]->rg == NONE)
                        codes.push_back(string("lw ")+GetRegName(RegMap[t.dst]->rg)+", "+address(t.src1));
                    else
                        codes.push_back(string("move ")+GetRegName(RegMap[t.dst]->rg)+", "+GetRegName(RegMap[t.src1]->rg));
                }
                break;
            }
            case SETL: {
                codes.push_back(string("Label_")+t.dst->name()+":");// 标签名为Label_txx
                break;
            }
            case FUNC: {
                codes.push_back(t.dst->name()+":"); // 函数入口为函数名， 例如sum:  main:
                // 参数部分的栈空间开辟由调用者进行，故这里开辟的栈空间不包括参数
                int tosub = RegMap[t.dst]->stackSize - 4*t.dst->paramCount();
                codes.push_back(string("subi $sp, $sp, ")+itos(tosub));
                if(t.dst->name()!="main")
                {
                    // store $ra and $fp
                    codes.push_back(string("sw $ra, ")+itos(tosub-4)+"($sp)");
                    codes.push_back(string("sw $fp, ")+itos(tosub-8)+"($sp)");
                    codes.push_back(string("move $fp, $sp"));
                    // store $s0-$s7 if necessary
                    for(vector<Reg>::iterator it = RegMap[t.dst]->funcUsed.begin(); it != RegMap[t.dst]->funcUsed.end(); it++)
                    {
                        codes.push_back(string("sw ") + GetRegName(*it) + ", " +
                        itos(tosub - 4 * (3 + it - RegMap[t.dst]->funcUsed.begin())) + "($fp)");
                    }
                }
                else
                {
                    codes.push_back(string("sw $ra, ")+itos(tosub-4)+"($sp)");
                    codes.push_back(string("move $fp, $sp"));
                }
                break;
            }
            case PARA: {
                break; // 暂时参数通过压栈传递，不进行寄存器分配，因此不需要做什么
            }
            case PUSH: {
                codes.push_back(string("subi $sp, $sp, 4"));
                if(IsConst(t.dst))// PUSH 2
                {
                    TempAlloc(t.dst);
                    codes.push_back(string("li ") + GetRegName(RegMap[t.dst]->rg) + ", " + itos(t.dst->value()));
                    codes.push_back(string("sw ") + GetRegName(RegMap[t.dst]->rg) + ", " + "0($sp)");
                    Refresh(RegMap[t.dst]->rg);
                }
                else// push t1
                {
                    if(RegMap[t.dst]->rg == NONE)
                    {
                        TempAlloc(t.dst);
                        codes.push_back(string("lw ")+GetRegName(RegMap[t.dst]->rg)+", "+address(t.dst));
                        codes.push_back(string("sw ")+GetRegName(RegMap[t.dst]->rg)+", 0($sp)");
                        Refresh(RegMap[t.dst]->rg);
                    }
                    else
                    {
                        codes.push_back(string("sw ")+GetRegName(RegMap[t.dst]->rg)+", 0($sp)");
                    }
                }
                break;
            }
            case CALL: {
                TempRefresh();
                codes.push_back(string("jal ")+t.dst->name());
                break;
            }
            case JMP: {
                TempRefresh();
                codes.push_back(string("j ")+"Label_"+t.dst->name());// j Label_txx
                break;
            }
            case NEG: {// NEG t1 t2
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                if(RegMap[t.dst]->rg == NONE)
                    TempAlloc(t.dst);
                codes.push_back(string("neg ")+GetRegName(RegMap[t.dst]->rg)+", "+GetRegName(RegMap[t.src1]->rg));
                break;
            }
            case RET: {// RET t; or RET;
                if(t.dst != NULL)
                {
                    if(RegMap[t.dst]->rg == NONE)
                        codes.push_back(string("lw $v0, ")+address(t.dst));
                    else
                        codes.push_back(string("move $v0, ")+GetRegName(RegMap[t.dst]->rg));
                }
                break;
            }
            case GET_RET: {//GET_RET t means t gets $v0
                if(RegMap[t.dst]->rg == NONE)
                    codes.push_back(string("sw $v0, ")+address(t.dst));
                else
                    codes.push_back(string("move ")+GetRegName(RegMap[t.dst]->rg)+", $v0");
                break;
            }
            case GIV_ARR: { // a[2] = t
                #ifdef mipsDEBUG
                cout<<"array to give:"<<t.dst->name()<<endl;
                #endif // mipsDEBUG
                if(IsConst(t.src2))
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src2]->rg)+", "+itos(t.src2->value()));
                }
                else if(RegMap[t.src2]->rg == NONE)
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src2]->rg)+", "+address(t.src2));
                }
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                codes.push_back(string("sll ") + GetRegName(RegMap[t.src1]->rg) + ", " + GetRegName(RegMap[t.src1]->rg) + ", 2");
                #ifdef mipsDEBUG
                cout<<"Global array:"<<t.dst->name()<<RegMap[t.dst]->isGlobal<<endl;
                #endif // mipsDEBUG
                if(RegMap[t.dst]->isGlobal)
                {
                    codes.push_back(string("add ") + GetRegName(RegMap[t.src1]->rg) + ", " + GetRegName(RegMap[t.src1]->rg) + ", $gp");
                    codes.push_back(string("sw ") + GetRegName(RegMap[t.src2]->rg) + ", " +
                    itos(RegMap[t.dst]->offsetByGp)+"("+GetRegName(RegMap[t.src1]->rg)+")");
                }
                else
                {
                    codes.push_back(string("add ") + GetRegName(RegMap[t.src1]->rg) + ", " + GetRegName(RegMap[t.src1]->rg) + ", $fp");
                    codes.push_back(string("sw ") + GetRegName(RegMap[t.src2]->rg) + ", " +
                    itos(RegMap[t.dst]->offsetByFp)+"("+GetRegName(RegMap[t.src1]->rg)+")");
                }
                break;
            }
            case ARR_GIV: {//t = a[x]
                if(RegMap[t.dst]->rg == NONE)
                    TempAlloc(t.dst);
                if(IsConst(t.src2))
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src2]->rg)+", "+itos(t.src2->value()));
                }
                else if(RegMap[t.src2]->rg == NONE)
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src2]->rg)+", "+address(t.src2));
                }
                codes.push_back(string("sll ") + GetRegName(RegMap[t.src2]->rg) + ", " + GetRegName(RegMap[t.src2]->rg) + ", 2");
                if(RegMap[t.src1]->isGlobal)
                {
                    codes.push_back(string("add ") + GetRegName(RegMap[t.src2]->rg) + ", " + GetRegName(RegMap[t.src2]->rg) + ", $gp");
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.dst]->rg)+", "+
                    itos(RegMap[t.src1]->offsetByGp)+"("+GetRegName(RegMap[t.src2]->rg)+")");
                }
                else
                {
                    codes.push_back(string("add ") + GetRegName(RegMap[t.src2]->rg) + ", " + GetRegName(RegMap[t.src2]->rg) + ", $fp");
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.dst]->rg)+", "+
                    itos(RegMap[t.src1]->offsetByFp)+"("+GetRegName(RegMap[t.src2]->rg)+")");
                }
                break;
            }
            case BEQ: {
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                if(IsConst(t.src2))
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src2]->rg)+", "+itos(t.src2->value()));
                }
                else if(RegMap[t.src2]->rg == NONE)
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src2]->rg)+", "+address(t.src2));
                }
                Reg t1 = RegMap[t.src1]->rg;
                Reg t2 = RegMap[t.src2]->rg;
                TempRefresh();
                codes.push_back(string("beq ")+GetRegName(t1)+", "+GetRegName(t2)+", Label_"+t.dst->name());
                break;
            }
            case BNE: {
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                if(IsConst(t.src2))
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src2]->rg)+", "+itos(t.src2->value()));
                }
                else if(RegMap[t.src2]->rg == NONE)
                {
                    TempAlloc(t.src2);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src2]->rg)+", "+address(t.src2));
                }
                Reg t1 = RegMap[t.src1]->rg;
                Reg t2 = RegMap[t.src2]->rg;
                TempRefresh();
                codes.push_back(string("bne ")+GetRegName(t1)+", "+GetRegName(t2)+", Label_"+t.dst->name());
                break;
            }
            case BGEZ: { // BGEZ LABEL src1  bgez $s0, label
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                Reg t1 = RegMap[t.src1]->rg;
                TempRefresh();
                codes.push_back(string("bgez ")+GetRegName(t1)+", Label_"+t.dst->name());
                break;
            }
            case BGTZ: {
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                Reg t1 = RegMap[t.src1]->rg;
                TempRefresh();
                codes.push_back(string("bgtz ")+GetRegName(t1)+", Label_"+t.dst->name());
                break;
            }
            case BLEZ: {
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                Reg t1 = RegMap[t.src1]->rg;
                TempRefresh();
                codes.push_back(string("blez ")+GetRegName(t1)+", Label_"+t.dst->name());
                break;
            }
            case BLTZ: {
                if(IsConst(t.src1))
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("li ")+GetRegName(RegMap[t.src1]->rg)+", "+itos(t.src1->value()));
                }
                else if(RegMap[t.src1]->rg == NONE)
                {
                    TempAlloc(t.src1);
                    codes.push_back(string("lw ")+GetRegName(RegMap[t.src1]->rg)+", "+address(t.src1));
                }
                Reg t1 = RegMap[t.src1]->rg;
                TempRefresh();
                codes.push_back(string("bltz ")+GetRegName(t1)+", Label_"+t.dst->name());
                break;
            }
            case WRITE: {// WRITE dst
                if(IsConst(t.dst))
                {
                    codes.push_back(string("li $a0, ")+itos(t.dst->value()));
                    if(t.dst->type() == CONST_INT)
                        codes.push_back(string("li $v0, 1"));//write 2
                    else
                        codes.push_back(string("li $v0, 11"));//write 'a'
                    codes.push_back(string("syscall"));
                }
                else if(t.dst->type() == CONST_STR)  // write "hello world"
                {
                    codes.push_back(string("la $a0, ")+t.dst->name());
                    codes.push_back(string("li $v0, 4"));
                    codes.push_back(string("syscall"));
                }
                else if(t.dst->type() == CHAR) // char a = 'a' + 2; write a (it's 'c')
                {
                    if(RegMap[t.dst]->rg == NONE)
                        codes.push_back(string("lw $a0, ")+address(t.dst));
                    else
                        codes.push_back(string("move $a0, ")+GetRegName(RegMap[t.dst]->rg));
                    codes.push_back(string("li $v0, 11"));
                    codes.push_back(string("syscall"));
                }
                else if(IsArray(t.dst))  // 数组的输出
                {
                    if(RegMap[t.dst]->isGlobal)
                        codes.push_back(string("addi $a1, $fp, ")+itos(RegMap[t.dst]->offsetByGp));
                    else
                        codes.push_back(string("addi $a1, $fp, ")+itos(RegMap[t.dst]->offsetByFp));
                    codes.push_back(string("addi $a2, $a1, ")+itos(4*t.dst->length()));
                    codes.push_back(string("Array_Print_")+t.dst->name()+":");
                    codes.push_back(string("lw $a0, 0($a1)"));
                    if(t.dst->type() == INT_ARR)
                        codes.push_back(string("li $v0, 1"));
                    else
                        codes.push_back(string("li $v0, 11"));
                    codes.push_back(string("syscall"));
                    codes.push_back(string("addi $a1, $a1, 4"));
                    codes.push_back(string("bne $a1, $a2, Array_Print_")+t.dst->name());
                }
                else//其余输出均按整数处理
                {
                    if(RegMap[t.dst]->rg == NONE)
                        codes.push_back(string("lw $a0, ")+address(t.dst));
                    else
                        codes.push_back(string("move $a0, ")+GetRegName(RegMap[t.dst]->rg));
                    codes.push_back(string("li $v0, 1"));
                    codes.push_back(string("syscall"));
                }
                break;
            }
            case READ: {//READ dst
                if(t.dst->type() == INT)
                {
                    codes.push_back(string("li $v0, 5"));
                    codes.push_back(string("syscall"));
                    if(RegMap[t.dst]->rg == NONE)
                        codes.push_back(string("sw $v0, ")+address(t.dst));
                    else
                        codes.push_back(string("move ")+GetRegName(RegMap[t.dst]->rg)+", $v0");
                }
                else if(t.dst->type() == CHAR)
                {
                    codes.push_back(string("li $v0, 12"));
                    codes.push_back(string("syscall"));
                    if(RegMap[t.dst]->rg == NONE)
                        codes.push_back(string("sw $v0, ")+address(t.dst));
                    else
                        codes.push_back(string("move ")+GetRegName(RegMap[t.dst]->rg)+", $v0");
                }
                break;
            }
            case END_FUNC: {
                int tosub = RegMap[t.dst]->stackSize - 4*t.dst->paramCount();
                codes.push_back(string("lw $ra, ")+itos(tosub-4)+"($sp)");
                if(t.dst->name()!="main")
                {
                    TempRefresh(false);
                    // restore $ra and $fp
                    codes.push_back(string("lw $fp, ")+itos(tosub-8)+"($sp)");
                    // restore $s0-$s7 if necessary
                    for(vector<Reg>::iterator it = RegMap[t.dst]->funcUsed.begin(); it != RegMap[t.dst]->funcUsed.end(); it++)
                    {
                        codes.push_back(string("lw ") + GetRegName(*it) + ", " +
                        itos(tosub - 4 * (3 + it - RegMap[t.dst]->funcUsed.begin())) + "($sp)");
                    }
                }
                codes.push_back(string("addi $sp, $sp, ")+itos(RegMap[t.dst]->stackSize));
                codes.push_back(string("jr $ra"));
                break;
            }
            case END_PROC: {
                codes.push_back(string("lw $ra, ")+itos(RegMap[t.dst]->stackSize-4)+"($sp)");
                codes.push_back(string("addi $sp, $sp, ")+itos(RegMap[t.dst]->stackSize));
                codes.push_back(string("jr $ra"));
                break;
            }
            default: break;
        }
    }
}

string MIPSTranslator::address(TableItem *t)
{
    if(RegMap[t]->isGlobal)
        return itos(RegMap[t]->offsetByGp) + "($gp)";
    else
        return itos(RegMap[t]->offsetByFp) + "($fp)";
}
