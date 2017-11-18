#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED
#include <iostream>
#include <map>
#include <string>
#include "global.h"
#include "TableItem.h"
using namespace std;

class TableItem;
class SymbolTable {
public:
    TableItem* Insert(string name, TableItemType type, int value);
    TableItem* GetItem(string name);
    SymbolTable();
private:
    map<string, TableItem*> table;
};

#endif // SYMBOLTABLE_H_INCLUDED
