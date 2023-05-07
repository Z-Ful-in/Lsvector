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
    bool put(K k, V v);//���룬�����ظ��ʱ�Ȼ�ɹ�
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
        p = p->pred;//�س�һ��
        if (p->pred && (p->entry.key == k)) return true;
        qlist = qlist->succ;
        if (!qlist->succ) return false;//�Ѿ��������trailer,ʧ��
        p = (p->pred) ? p->below : qlist->data->first();//ת����һ��(p�Ѿ���ͷ�ڱ���Ҫת����һ���ͷ�ڱ�)
    }
}
template<typename K, typename V> bool Skiplist<K, V>::put(K k, V v)
{
    Entry<K, V> e = Entry<K, V>(k, v);
    if (empty()) insertAsFirst(new Quadlist<Entry<K, V>>);//�����׸�Entry���ײ㣩
    ListNode<Quadlist<Entry<K, V>>*>* qlist = first();
    QuadlistNode<Entry<K, V>>* p = qlist->data->first();
    if (skipSearch(qlist, p, k))
        while (p->below) p = p->below;
    qlist = last();
    QuadlistNode<Entry<K, V>>* b = qlist->data->insertAfterAbove(e, p);//����ײ��ϲ����µĻ���
    while (rand() & 1)
    {
        while (qlist->data->valid(p) && !p->above) p = p->pred;//�ҵ���һ������ߵ�ǰ��
        if (!qlist->data->valid(p))
        {
            if (qlist == first())//��Ҫ������Ѿ�����߲�ʱ
                insertAsFirst(new Quadlist<Entry<K, V>>);//�¼�һ��
            p = qlist->pred->data->first()->pred;//ת���¼Ӳ��header
        }
        else
            p = p->above;
        qlist = qlist->pred;//����
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
        p = lower; qlist = qlist->succ;//��¼����������ɾ��
    } while (qlist->succ);
    while (!empty() && first()->data->empty())//���QuadlistΪ�գ�ɾ��
        List::remove(first());
    return true;
}

