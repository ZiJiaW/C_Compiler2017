#include "TableItem.h"

TableItem::TableItem(string name, TableItemType type, int value):
    _name(name), _type(type), _value(value), _paramCount(-1), _funcField(NULL), _arrayLen(-1), _array(NULL), useCount(0){}

string TableItem::name()
{
    return _name;
}
TableItemType TableItem::type()
{
    return _type;
}
int TableItem::value()
{
    return _value;
}
int TableItem::paramCount()
{
    return _paramCount;
}
SymbolTable* TableItem::funcField()
{
    return _funcField;
}
int TableItem::length()
{
    return _arrayLen;
}
int *TableItem::array()
{
    return _array;
}
void TableItem::SetField(SymbolTable *f)
{
    _funcField = f;
}
/**
 * set array length
 * open a memory to store array
*/
void TableItem::SetLength(int len)
{
    _arrayLen = len;
    _array = new int(len);
}
void TableItem::SetParamCount(int cot)
{
    _paramCount = cot;
}
void TableItem::SetValue(int value)
{
    _value = value;
}
