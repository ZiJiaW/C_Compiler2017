#ifndef TABLEITEM_H_INCLUDED
#define TABLEITEM_H_INCLUDED
#include <string>
#include <iostream>
#include "global.h"
#include "SymbolTable.h"
using namespace std;
class SymbolTable;
class TableItem {
public:
    TableItem(string name, TableItemType type, int value);

    string name();
    TableItemType type();
    int value();
    int paramCount();
    SymbolTable* funcField();
    int length();

    void SetValue(int value);
    void SetParamCount(int cot);
    void SetLength(int len);
    void SetField(SymbolTable *f);
private:
    string _name;
    TableItemType _type;
    int _value;
    int _paramCount;
    SymbolTable *_funcField;
    int _arrayLen;
};


#endif // TABLEITEM_H_INCLUDED
