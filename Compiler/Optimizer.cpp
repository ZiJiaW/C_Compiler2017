#include <algorithm>
#include "Optimizer.h"

Optimizer::Optimizer(MiddleCode &_mc, SymbolTable &_tbl, vector<TableItem*> &_tmptbl):
mc(_mc), tbl(_tbl), tmptbl(_tmptbl){}


void Optimizer::optimize()
{
    PeepHoleOpt();
    DAGOpt();
    PeepHoleOpt();
}
bool Optimizer::IsConst(TableItem* t)
{
    return t->type()==CONST_CHAR || t->type()==CONST_INT;
}
bool Optimizer::AbleOpt(midInstr md)
{
    return md.op == ADD || md.op == SUB || md.op == DIV || md.op == MUL
        || md.op == GIV || md.op == NEG ;
}
void Optimizer::PeepHoleOpt()
{
    result = mc.code;
    while(true)
    {
        bool opted = false;
        vector<midInstr> ret;
        for(vector<midInstr>::iterator p = result.begin(); p != result.end(); p++)
        {
            if(p->op == GIV && IsTemp(p->dst) && ((p+1)->src1 == p->dst || (p+1)->src2 == p->dst) && AbleOpt(*(p+1)))
            {
                TableItem* src1 = (p+1)->src1 == p->dst ? p->src1 : (p+1)->src1;
                TableItem* src2 = (p+1)->src2 == p->dst ? p->src1 : (p+1)->src2;
                ret.push_back(midInstr((p+1)->op, (p+1)->dst, src1, src2));
                ++p;
                opted = true;
                continue;
            }
            else if((p+1)->op == GIV && IsTemp(p->dst) && (p+1)->src1==p->dst && IsCalc(*p))
            {
                ret.push_back(midInstr(p->op, (p+1)->dst, p->src1, p->src2));
                opted = true;
                ++p;
                continue;
            }
            else if(IsCalc(*p) && p->op != GIV && p->op != NEG && IsConst(p->src1) && IsConst(p->src2))
            {
                opted = true;
                TableItem* t = p->dst;
                SymbolTable* curTbl = NULL;
                for(map<string, TableItem*>::iterator q = tbl.table.begin(); q != tbl.table.end(); q++)
                {
                    if(q->second->funcField() != NULL)
                    {
                        if(q->second->funcField()->table.find(t->name()) != q->second->funcField()->table.end())
                        {
                            curTbl = q->second->funcField();
                            break;
                        }
                        else if(find(q->second->funcField()->temp.begin(), q->second->funcField()->temp.end(), t)
                                != q->second->funcField()->temp.end())
                        {
                            curTbl = q->second->funcField();
                            break;
                        }
                    }
                }
                int val = p->op==ADD ? p->src1->value()+p->src2->value() :
                          p->op==SUB ? p->src1->value()-p->src2->value() :
                          p->op==MUL ? p->src1->value()*p->src2->value() :
                          p->op==DIV ? p->src1->value()/p->src2->value() : 0;
                TableItem* toadd = new TableItem(p->dst->name()+"_temp", CONST_INT, val);
                tmptbl.push_back(toadd);
                (curTbl->temp).push_back(toadd);
                ret.push_back(midInstr(GIV, p->dst, toadd));
                continue;
            }
            else if(p->op == GIV && p->dst == p->src1)
            {
                opted = true;
                continue;
            }
            ret.push_back(*p);
        }
        if(opted)
            result = ret;
        else
            break;
    }
    mc.code = result;
}

/**判断是否是基本块中的指令*/
bool Optimizer::IsCalc(midInstr md)
{
    return md.op == ADD || md.op == SUB || md.op == DIV || md.op == MUL
        || md.op == GIV || md.op == NEG;
}
/**
 * extend DAG by analyze middle code;
*/
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
        for(vector<int>::iterator it = nodes[s].fathers.begin(); it != nodes[s].fathers.end(); it++)
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
/**
 * 根据得到的Map生成反向索引
*/
void Graph::GenIndexRecord()
{
    indexRecord.resize(nodes.size());
    for(map<TableItem*, int>::iterator p = record.begin(); p != record.end(); p++)
    {
        indexRecord[p->second].push_back(p->first);
    }
}
/**
 * get TableItem by index(int)
*/
TableItem* Graph::GetItemByIndex(int index)
{
    if(index == -1)
        return NULL;
    else
        return nodes[index].isLeaf ? nodes[index].val : indexRecord[index][0];
}
/**
 * judge if t is a temp variable;
*/
bool Optimizer::IsTemp(TableItem* t)
{
    return find(tmptbl.begin(), tmptbl.end(), t) != tmptbl.end();
}
/**
 * do optimize by construct a DAG
*/
void Optimizer::DAGOpt()
{
    vector<midInstr> block;
    result.clear();
    for(vector<midInstr>::iterator it = mc.code.begin(); it != mc.code.end(); it++)
    {
        #ifdef OPTDEBUG
        cout<<string(it->dst!=NULL?it->dst->name():"none")<<endl;
        #endif // OPTDEBUG
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
                    {
                        curTbl = p->second->funcField();
                        break;
                    }
                    else if(find(p->second->funcField()->temp.begin(), p->second->funcField()->temp.end(), t)
                            != p->second->funcField()->temp.end())
                    {
                        curTbl = p->second->funcField();
                        break;
                    }
                }
            }
            // 建立DAG图
            Graph graph;
            vector<TableItem*> givens;// 记录被复制过的变量
            map<TableItem*, int> useCount;// 记录每个变量使用次数
            for(vector<midInstr>::iterator p = block.begin(); p != block.end(); p++)
            {
                graph.Extend(*p);
                givens.push_back(p->dst); // 记录被赋值过的变量
                // 记录每个变量的出现次数
                if(p->dst != NULL)
                    useCount[p->dst] = useCount.find(p->dst) != useCount.end() ? useCount[p->dst]+1 : 1;
                if(p->src1 != NULL)
                    useCount[p->src1] = useCount.find(p->src1) != useCount.end() ? useCount[p->src1]+1 : 1;
                if(p->src2 != NULL)
                    useCount[p->src2] = useCount.find(p->src2) != useCount.end() ? useCount[p->src2]+1 : 1;
            }
            graph.GenIndexRecord();
            // 如果没有中间节点的共享，则没有公共表达式，不需要优化
            bool isToOpt = false;
            for(int i = 0; i < graph.nodes.size(); ++i)
            {
                if(graph.indexRecord[i].size()>1)
                    isToOpt = true;
            }
            if(!isToOpt)
            {
                for(vector<midInstr>::iterator p = block.begin(); p != block.end(); ++p)
                    result.push_back(*p);
                result.push_back(*it);
                block.clear();
                continue;
            }
            // 如果作为叶子节点的变量被重新赋值过，则需要生成a0 = a之类的预赋值
            for(vector<Node>::iterator p = graph.nodes.begin(); p != graph.nodes.end(); p++)
            {
                if(p->isLeaf && find(givens.begin(), givens.end(), p->val) != givens.end())
                {
                    TableItem* newtmp = new TableItem(p->val->name()+"_tmp", TMP);
                    curTbl->temp.push_back(newtmp);
                    tmptbl.push_back(newtmp);
                    result.push_back(midInstr(GIV, newtmp, p->val));// 导出的初始代码为 a0 = a
                    p->val = newtmp;// 之后替换原叶子节点值为 a0
                }
            }
            // 防止预赋值指令被优化掉，加一条无效指令
            result.push_back(midInstr(STR, NULL));
            // 下面确定中间代码导出顺序
            vector<int> out;
            vector<bool> exported(graph.nodes.size(), false);
            while(out.size() != graph.nodes.size())
            {
                for(vector<Node>::iterator p = graph.nodes.begin(); p != graph.nodes.end(); p++)
                {
                    bool ok = true;
                    for(vector<int>::iterator it = p->fathers.begin(); it != p->fathers.end(); it++)
                        ok &= exported[*it];
                    if(ok && exported[p-graph.nodes.begin()]==false)
                    {
                        out.push_back(p-graph.nodes.begin());
                        exported[p-graph.nodes.begin()] = true;
                    }
                    if(out.size() == graph.nodes.size())
                        break;
                    #ifdef OPTDEBUG
                    //for(vector<bool>::iterator it = exported.begin(); it != exported.end(); it++)
                    //    cout<<*it<<' ';
                    //cout<<out.size()<<' '<<graph.nodes.size()<<endl;
                    #endif // OPTDEBUG
                }
            }
            // 根据上面的顺序进行代码导出
            for(vector<int>::reverse_iterator p = out.rbegin(); p != out.rend(); p++)
            {
                bool has = false;
                vector<TableItem*> toErase;
                for(vector<TableItem*>::iterator it = graph.indexRecord[*p].begin(); it != graph.indexRecord[*p].end(); it++)
                {
                    if(graph.nodes[*p].isLeaf)// 对于叶子节点，作赋值操作
                    {
                        if((IsTemp(*it) && (useCount[*it] == 2)) || ((*it)->type() == CONST_CHAR || (*it)->type() == CONST_INT))
                            continue;
                        result.push_back(midInstr(GIV, *it, graph.nodes[*p].val));
                    }
                    else if(graph.indexRecord[*p].size() == 1)
                    {
                        result.push_back(midInstr(graph.nodes[*p].op, *it,
                        graph.GetItemByIndex(graph.nodes[*p].leftChild),
                        graph.GetItemByIndex(graph.nodes[*p].rightChild)));
                    }
                    else
                    {
                        if(IsTemp(*it) && useCount[*it] == 2 && (it+1 != graph.indexRecord[*p].end() || has))
                        {
                            toErase.push_back(*it);
                            continue;
                        }
                        has = true;
                        result.push_back(midInstr(graph.nodes[*p].op, *it,
                        graph.GetItemByIndex(graph.nodes[*p].leftChild),
                        graph.GetItemByIndex(graph.nodes[*p].rightChild)));
                    }
                }
                // 删除节点的冗余
                for(vector<TableItem*>::iterator it = toErase.begin(); it != toErase.end(); it++)
                {
                    graph.indexRecord[*p].erase(find(graph.indexRecord[*p].begin(), graph.indexRecord[*p].end(), *it));
                }
            }

            result.push_back(*it);
            block.clear();
        }
    }
    mc.code = result;
}
