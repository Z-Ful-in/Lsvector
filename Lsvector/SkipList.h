#include"LsList.h"
#include"Entry.h"
#include"Quadlist.h"
#include"Dictionary.h"
template<typename K, typename V> class Skiplist :public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*>
{
protected:
    bool skipSearch(ListNode<Quadlist<Entry<K, V>>*>*& qlist, QuadlistNode<Entry<K, V>>*& p, K& k);
public:
    int size() const { return empty() ? 0 : last()->data->size(); }
    int level() { return List:; size(); }
    bool put(K k, V v);//插入，允许重复故必然成功
    V* get(K k);
    bool remove(K k);
};
template<typename K, typename V> V* Skiplist<K, V>::get(K k)
{
    if (empty()) return NULL;
    ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
    QuadlistNode<Entry<K, V>>* p = qlist->data->first();
    return skipSearch(qlist, p, k) ? &(p->entry.value) : NULL;
}
template<typename K, typename V> bool Skiplist<K, V>::skipSearch(ListNode<Quadlist<Entry<K, V>>*>*& qlist,
    QuadlistNode<Entry<K, V>>*& p, K& k)
{
    while (true)
    {
        while (p->succ && (p->entry.key <= k)) p = p->succ;
        p = p->pred;//回撤一步
        if (p->pred && (p->entry.key == k)) return true;
        qlist = qlist->succ;
        if (!qlist->succ) return false;//已经是链表的trailer,失败
        p = (p->pred) ? p->below : qlist->data->first();//转到下一层(p已经是头哨兵需要转到下一层的头哨兵)
    }
}
template<typename K, typename V> bool Skiplist<K, V>::put(K k, V v)
{
    Entry<K, V> e = Entry<K, V>(k, v);
    if (empty()) insertAsFirst(new Quadlist<Entry<K, V>>);//插入首个Entry（首层）
    ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
    QuadlistNode<Entry<K, V>>* p = qlist->data->first();
    if (skipSearch(qlist, p, k))
        while (p->below) p = p->below;
    qlist = last();
    QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//在最底层上插入新的基座
    while (rand() & 1)
    {
        while (qlist->data->valid(p) && !p->above) p = p->pred;//找到第一个比其高的前驱
        if (!qlist->data->valid(p))
        {
            if (qlist == first())//需要升层而已经是最高层时
                insertAsFirst(new Quadlist<Entry<K, V>>);//新加一层
            p = qlist->pred->data->first()->pred;//转至新加层的header
        }
        else
            p = p->above;
        qlist = qlist->pred;//升层
        b = qlist->data->insertAfterAbove(e, p, b);
    }
    return true;
}
template<typename K, typename V> bool Skiplist<K, V>::remove(K k)
{
    if (empty()) return false;
    ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
    QuadlistNode<Entry<K, V>>* p = qlist->data->first();
    if (!skipSearch(qlist, p, k)) return false;
    do
    {
        QuadlistNode<Entry<K, V>>* lower = p->below;
        qlist->data->remove(p);
        p = lower; qlist = qlist->succ;//记录，向下深入删除
    } while (qlist->succ);
    while (!empty() && first()->data->empty())//如果Quadlist为空，删除
        List::remove(first());
    return true;
}

