#ifndef MIDDLECODE_H_INCLUDED
#define MIDDLECODE_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "global.h"
#include "TableItem.h"

class MiddleCode {
public:
    void Insert();
private:
    vector<midInstr> code;
};

#endif // MIDDLECODE_H_INCLUDED
