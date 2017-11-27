#ifndef TABLEITEM_H_INCLUDED
#define TABLEITEM_H_INCLUDED
#include <string>
#include <vector>
#include <iostream>
#include "global.h"
#include "SymbolTable.h"
using namespace std;
class SymbolTable;
class TableItem {
public:
    TableItem(string name, TableItemType type, int value = 0);

    string name();
    TableItemType type();
    int value();
    int paramCount();
    SymbolTable* funcField();
    int length();
    int *array();

    void SetValue(int value);
    void SetParamCount(int cot);
    void SetLength(int len);
    void SetField(SymbolTable *f);

    int useCount;

    vector<string> paraName; // 对于字符串常量，容器的第一个位置存储之
private:
    string _name;
    TableItemType _type;
    int _value;
    int _paramCount;
    SymbolTable *_funcField;
    int _arrayLen;
    int *_array; // array content(use int to store both int and char)
};


#endif // TABLEITEM_H_INCLUDED
