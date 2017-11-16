#include <iostream>
#include <string>
#include "Symbol.h"
using namespace std;
Symbol::Symbol(SymType type, int lineNum, string name, int numVal)
{
    _name = name;
    _type = type;
    _numVal = numVal;
    _lineNum = lineNum;
}
Symbol::Symbol(){}
int Symbol::lineNum()
{
    return _lineNum;
}
string Symbol::name()
{
    return _name;
}
SymType Symbol::type()
{
    return _type;
}
int Symbol::numVal()
{
    return _numVal;
}
