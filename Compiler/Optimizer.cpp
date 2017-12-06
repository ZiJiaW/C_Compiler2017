#include <algorithm>
#include "Optimizer.h"

Optimizer::Optimizer(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl):
mc(_mc), tbl(_tbl), tmptbl(_tmptbl){}


void Optimizer::optimize()
{

}

void Optimizer::PeepHoleOpt()
{

}

/*判断是否是基本块中的指令*/
bool Optimizer::IsCalc(midInstr md)
{
    return md.op == ADD || md.op == SUB || md.op == DIV || md.op == MUL
        || md.op == GIV || md.op == ARR_GIV || md.op == GIV_ARR || md.op == NEG;
}

void Graph::Extend(midInstr md)
{
    // op dst src1 src2
    if(md.op == ADD || md.op == SUB || md.op == DIV
    || md.op == MUL || md.op == ARR_GIV || md.op == GIV_ARR)
    {
        int s1 = Insert(md.src1);
        int s2 = Insert(md.src2);
        vector<int> fs1;
        vector<int> fs2;
        for(vector<int>::iterator it = nodes[s1].fathers.begin(); it != nodes[s1].fathers.end(); it++)
        {
            if(nodes[*it].op == md.op)
                fs1.push_back(*it);
        }
        for(vector<int>::iterator it = nodes[s2].fathers.begin(); it != nodes[s2].fathers.end(); it++)
        {
            if(nodes[*it].op == md.op)
                fs2.push_back(*it);
        }
        for(vector<int>::iterator p = fs1.begin(); p != fs1.end(); p++)
        {
            for(vector<int>::iterator q = fs2.begin(); q != fs2.end(); q++)
            {
                if(*p == *q)
                {
                    record[md.dst] = *q;
                    return;
                }
            }
        }
        nodes.push_back(Node(false, NULL, md.op));
        int index = nodes.size() - 1;
        record[md.dst] = index;
        nodes[index].leftChild = s1;
        nodes[index].rightChild = s2;
        nodes[s1].fathers.push_back(index);
        nodes[s2].fathers.push_back(index);
    }
    // dst = -src1
    else if(md.op == NEG)
    {
        int s = Insert(md.src1);
        for(vector<int>::iterator it = nodes[s1].fathers.begin(); it != nodes[s1].fathers.end(); it++)
        {
            if(nodes[*it].op == md.op)
            {
                record[md.dst] = *it;
                return;
            }
        }
        nodes.push_back(Node(false, NULL, md.op));
        int index = nodes.size() - 1;
        record[md.dst] = index;
        nodes[index].leftChild = s;
        nodes[s].fathers.push_back(index);
    }
    // dst = src1
    else if(md.op == GIV)
    {
        int s = Insert(md.src1);
        record[md.dst] = s;
    }
}
/**
 * find index of t, return it's index;
 * if not exists, insert it and return it's index;
*/
int Graph::Insert(TableItem* t)
{
    map<TableItem*, int>::iterator it = record.find(t);
    if(it == record.end())
    {
        nodes.push_back(Node(true, t));
        record[t] = nodes.size() - 1;
        return record[t];
    }
    else
        return it->second;
}
void Optimizer::DAGOpt()
{
    vector<midInstr> block;
    for(vector<midInstr>::iterator it = mc.code.begin(); it != mc.code.end(); it++)
    {
        if(!IsCalc(*it) && block.empty())
        {
            result.push_back(*it);
            continue;
        }
        else if(IsCalc(*it))
        {
            block.push_back(*it);
            continue;
        }
        else // 处理当前基本块
        {
            // 获得当前所在函数域
            TableItem* t = block[0].dst;
            SymbolTable* curTbl = NULL;
            for(map<string, TableItem*>::iterator p = tbl.table.begin(); p != tbl.table.end(); p++)
            {
                if(p->second->funcField() != NULL)
                {
                    if(p->second->funcField()->table.find(t->name()) != p->second->funcField()->table.end())
                        curTbl = p->second->funcField();
                    else if(find(p->second->funcField()->temp.begin(), p->second->funcField()->temp.end(), t)
                    != p->second->funcField()->temp.end())
                        curTbl = p->second->funcField();
                }
            }
            // 建立DAG图
            Graph graph;
            vector<TableItem*> givens;// 记录被复制过的变量
            for(vector<midInstr>::iterator p = block.begin(); p != block.end(); p++)
            {
                graph.Extend(*p);
                givens.push_back(p->dst);
            }
            // 如果作为叶子节点的变量被重新赋值过，则需要生成a0 = a之类的预赋值
            for(vector<Node>::iterator p = graph.nodes.begin(); p != graph.nodes.end(); p++)
            {
                if(p->isLeaf)
                {
                    vector<TableItem*>::iterator it = find(givens.begin(), givens.end(), p->val);
                    if(it != givens.end())
                    {

                    }
                }
            }
        }
    }
}