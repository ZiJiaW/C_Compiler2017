#include <iostream>
#include <string>
#include "Symbol.h"
using namespace std;
Symbol::Symbol(string _name, SymType _type, int _numValue, int _lineNum)
{
    name = _name;
    type = _type;
    numVal = _numVal;
    lineNum = _lineNum;
}
Symbol::lineNum()
{
    return lineNum;
}
Symbol::name()
{
    return name;
}
Symbol::type()
{
    return type;
}
Symbol::numVal()
{
    return numVal;
}
