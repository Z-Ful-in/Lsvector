#ifndef QUADLISTNODE_H_INCLUDED
#define QUADLISTNODE_H_INCLUDED

#include<iostream>
#include"Entry.h"
#define QlistNodePosi(T) QuadlistNode<T>*
template<typename T>
struct QuadlistNode
{
    T entry;
    QlistNodePosi(T) pred; QlistNodePosi(T) succ;//ǰ�����
    QlistNodePosi(T) above; QlistNodePosi(T) below;//��������
    QuadlistNode(T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
        QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL)
        :entry(e), pred(p), succ(s), above(a), below(b) {}
    QlistNodePosi(T) insertAsSuccAbove(T const& e, QlistNodePosi(T) b = NULL);
    //�����½ڵ�,�Ե�ǰ�ڵ�Ϊǰ��,�Խڵ�bΪ����
};
template <typename T> QlistNodePosi(T)//��e��Ϊ��ǰ�ڵ�ĺ�̣�b�����ڲ���Quadlist
QuadlistNode<T>::insertAsSuccAbove(T const& e, QlistNodePosi(T) b)//���ڶ��ں���Ĭ��ֵ��������
{                                                     //ֻ��Ҫ�ڶ����ʱ�򣬽����������У���ʵ�ֵ�ʱ�򣬲���Ҫ��дһ��
    QlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);//�����½ڵ�
    succ->pred = x; succ = x;//����ˮƽ��������
    if (b) b->above = x;//���ô�ֱ��������
    return x;//�����½ڵ��λ��
}

#endif // QUADLISTNODE_H_INCLUDED
