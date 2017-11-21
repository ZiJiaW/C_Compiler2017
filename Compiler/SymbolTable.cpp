#include "SymbolTable.h"

SymbolTable::SymbolTable(): returnsth(0), returnnth(0){}

/**
1. insert a table item with name, type, value;
2. if it already exists, return null(ERROR);
3. on success, return new item pointer;
*/
TableItem* SymbolTable::Insert(string name, TableItemType type, int value)
{
    if(table.find(name) != table.end())
        return NULL;
    TableItem* newItem = new TableItem(name, type, value);
    table[name] = newItem;
    return newItem;
}

/**
* return an item pointer named name, if not exists, return NULL;
*/
TableItem* SymbolTable::GetItem(string name)
{
    map<string, TableItem*>::iterator it = table.find(name);
    return it == table.end() ? NULL : it->second;
}

