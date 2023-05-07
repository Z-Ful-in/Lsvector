#ifndef QUADLIST_H_INCLUDED
#define QUADLIST_H_INCLUDED

#include"QuadNode.h"
#define QlistNodePosi(T) QuadlistNode<T>*
template<typename T> class Quadlist
{
private:
    int _size;
    QlistNodePosi(T) header;
    QlistNodePosi(T) trailer;
protected:
    void init();
    int clear();
public:
    Quadlist() { init(); }
    ~Quadlist() { clear(); delete header; delete trailer; }
    int size() const { return _size; }
    bool empty() const { return _size <= 0; }
    QlistNodePosi(T) first() const { return header->succ; }
    QlistNodePosi(T) last() const { return trailer->pred; }
    bool valid(QlistNodePosi(T) p)
    {
        return p && (p != header) && (p != trailer);
    }
    T remove(QlistNodePosi(T) p);
    QlistNodePosi(T) insertAfterAbove(T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL);
};
template<typename T>
QlistNodePosi(T)
Quadlist<T>::insertAfterAbove(T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b) {
    _size++;
    return p->insertAsSuccAbove(e, b);//�����½ڵ�λ��(below)
}
template<typename T> void Quadlist<T>::init()//��ʼ��������Quadlistͳһ����
{
    header = new QuadlistNode<T>;//����Ͷ�ڱ��ڵ�
    trailer = new QuadlistNode<T>;//����β�ڱ��ڵ�
    header->succ = trailer;//�غ��������ڱ�
    header->pred = NULL;
    trailer->pred = header;//�غ��������ڱ�
    trailer->succ = NULL;
    header->above = trailer->above = NULL;//�����ǰ���ÿ�
    header->below = trailer->below = NULL;//����ĺ���ÿ�
    _size = 0;//��ʼ����ģ
}//�������������ʱ����ʵ�ʽڵ�,����������������ʱ����
template<typename T> T Quadlist<T>::remove(QlistNodePosi(T) p)
{
    p->pred->succ = p->succ; p->succ->pred = p->pred;
    _size--;
    T e = p->entry; delete p;
    return e;
}
template<typename T> int Quadlist<T>::clear()
{
    int oldsize = _size;
    while (_size > 0) remove(header->succ);
    return oldsize;
}

#endif // QUADLIST_H_INCLUDED
