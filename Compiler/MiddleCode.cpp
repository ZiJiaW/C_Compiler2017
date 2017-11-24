#include "MiddleCode.h"

void MiddleCode::Generate(OpCode _op, TableItem* _dst, TableItem* _src1, TableItem* _src2)
{
    midInstr m;
    m.op = _op;
    m.dst = _dst;
    m.src1 = _src1;
    m.src2 = _src2;
    code.push_back(m);
}
void MiddleCode::Concat(MiddleCode* md)
{
    code.insert(code.end(),md->code.begin(), md->code.end());
}