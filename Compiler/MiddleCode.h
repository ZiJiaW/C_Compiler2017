#ifndef MIDDLECODE_H_INCLUDED
#define MIDDLECODE_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "global.h"
#include "TableItem.h"
using namespace std;
struct midInstr {
    OpCode op;
    TableItem* dst;
    TableItem* src1;
    TableItem* src2;
};
class MiddleCode {
public:
    void Generate(OpCode op, TableItem *dst = NULL, TableItem *src1 = NULL, TableItem *src2 = NULL);
    vector<midInstr> code;
    void Concat(MiddleCode* md);
};
string GetOpString(OpCode op);
#endif // MIDDLECODE_H_INCLUDED
