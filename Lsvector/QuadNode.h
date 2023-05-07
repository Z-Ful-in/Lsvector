#ifndef QUADLISTNODE_H_INCLUDED
#define QUADLISTNODE_H_INCLUDED

#include<iostream>
#include"Entry.h"
#define QlistNodePosi(T) QuadlistNode<T>*
template<typename T>
struct QuadlistNode
{
    T entry;
    QlistNodePosi(T) pred; QlistNodePosi(T) succ;//前驱后继
    QlistNodePosi(T) above; QlistNodePosi(T) below;//上邻下邻
    QuadlistNode(T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
        QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL)
        :entry(e), pred(p), succ(s), above(a), below(b) {}
    QlistNodePosi(T) insertAsSuccAbove(T const& e, QlistNodePosi(T) b = NULL);
    //插入新节点,以当前节点为前驱,以节点b为下邻
};
template <typename T> QlistNodePosi(T)//将e作为当前节点的后继，b的上邻插入Quadlist
QuadlistNode<T>::insertAsSuccAbove(T const& e, QlistNodePosi(T) b)//对于对于函数默认值的声明，
{                                                     //只需要在定义的时候，进行声明就行，在实现的时候，不需要再写一遍
    QlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);//创建新节点
    succ->pred = x; succ = x;//设置水平逆向链接
    if (b) b->above = x;//设置垂直逆向链接
    return x;//返回新节点的位置
}

#endif // QUADLISTNODE_H_INCLUDED
