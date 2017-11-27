#include "MiddleCode.h"

void MiddleCode::Generate(OpCode _op, TableItem* _dst, TableItem* _src1, TableItem* _src2)
{
    midInstr m;
    m.op = _op;
    m.dst = _dst;
    if(_dst && (_dst->type() == INT || _dst->type() == CHAR))
        _dst->useCount++;
    m.src1 = _src1;
    if(_src1 && (_src1->type() == INT || _src1->type() == CHAR))
        _src1->useCount++;
    m.src2 = _src2;
    if(_src2 && (_src2->type() == INT || _src2->type() == CHAR))
        _src2->useCount++;
    code.push_back(m);
}
void MiddleCode::Concat(MiddleCode* md)
{
    code.insert(code.end(),md->code.begin(), md->code.end());
}