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
    return p->insertAsSuccAbove(e, b);//返回新节点位置(below)
}
template<typename T> void Quadlist<T>::init()//初始化，构造Quadlist统一调用
{
    header = new QuadlistNode<T>;//创建投哨兵节点
    trailer = new QuadlistNode<T>;//创建尾哨兵节点
    header->succ = trailer;//沿横向联结哨兵
    header->pred = NULL;
    trailer->pred = header;//沿横向联结哨兵
    trailer->succ = NULL;
    header->above = trailer->above = NULL;//纵向的前驱置空
    header->below = trailer->below = NULL;//纵向的后继置空
    _size = 0;//初始化规模
}//构造的四联表暂时不含实质节点,且与其他四联表暂时独立
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
