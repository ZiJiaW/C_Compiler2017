#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED
#include "MiddleCode.h"
#include "SymbolTable.h"
class Optimizer {
public:
    Optimizer(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl);
    void optimize(); // 外部接口，调用进行优化
private:
    void PeepHoleOpt(); // 窥孔优化
    void DAGOpt(); // DAG图优化
    bool IsCalc(midInstr md);
    
    MiddleCode &mc;
    SymbolTable &tbl;
    vector<TableItem*> &tmptbl;
    vector<midInstr> result; // 存放优化后的代码
    bool IsTemp(TableItem* t); // 是否是临时变量
    
};
// DAG图的结点
struct Node {
    TableItem* val; // 叶子节点是某变量的初值，直接放置某变量即可
    OpCode op; // 非叶子结点的内容是一种运算
    bool isLeaf;
    vector<int> fathers;// 父节点索引
    int leftChild;// 左孩子结点的索引，-1表示没有，如果只有一个孩子，则使用左孩子
    int rightChild;// 右孩子
    Node(bool isl = false, TableItem* t = NULL, OpCode _op = STR):
    isLeaf(isl), leftChild(-1), rightChild(-1), val(t), op(_op){}
};
struct Graph {
    vector<Node> nodes;// 结点集
    map<TableItem*, int> record;// 索引表
    void Extend(midInstr md);// 根据中间代码扩充DAG图
    int Insert(TableItem *t);
    vector<vector<TableItem*> > indexRecord;
    void GenIndexRecord();
    TableItem* GetItemByIndex(int index);
};
#endif // OPTIMIZER_H_INCLUDED