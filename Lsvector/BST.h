#pragma once
#include"LsBinTree.h"
#include"Lsrelease.h"
template<typename T>class BST :public BinTree<T> {
public:	virtual BinNodePosi(T)& search(const T&);//����鲻�������ջ�ָ�����սڵ����һλ�սڵ㣬����ʱ��_hot��Ϊ��̬��������ָ��սڵ�ĸ���
	virtual BinNodePosi(T) insert(const T&);
	virtual bool remove(const T&);

	static BinNodePosi(T)& searchIn(BinNodePosi(T) &v,const T&e,BinNodePosi(T)_hot) {
		if (!v || (e == v->data))return v;
		_hot = v;
		return searchIn((e < v->data ? v->lc : v->rc), e, _hot);//_hot������ָ�����սڵ��ĩ�ˣ������սڵ㣨�����Ƿ�鵽���ĸ��ס�
	}
protected:
	BinNodePosi(T) _hot;
	BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T));
};

template<typename T>BinNodePosi(T)& BST<T>::search(const T& e) { return searchIn(_root, e, _hot = NULL); }
template<typename T>BinNodePosi(T) BST<T>::insert(const T& e) {
	BinNodePosi(T) &x = search(e);//search�ķ���ֵ���������ͣ�������Ϊ��ֵ���з�װ
	if (!x) {//��ֹ������ͬ���
		x = new BinNodePosi(e, _hot);
		_size++; updateHeightAbove(x);
	}
	return x;
}
template<typename T>bool BST<T>::remove(T const& e) {
	BinNodePosi(T) &x = search(e);
	if (!x)return false;//������Ҳ��������ؿ�
	removeAt(x, _hot);
	_size--; updateHeightAbove(_hot);
	return true;
}
template<typename T>static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	BinNodePosi(T) w = x;
	BinNodePosi(T)succ = NULL;
	if (!HasLChild(*x))succ = x = x->rc;
	else if (!HasRChild(*x))succ = x = x->lc;
	else{
		w = w->succ; swap(x->data, w->data);//����x�����̣��������壩�ڵ�����ݣ���ʱ��w���ֻ��һ���Һ��ӣ�������
		BinNodePosi(T)u = w->parant;//�ҵ�w�ĸ��ף��������������ڵĻ����Һ��ӵ��游
		(u == x ? u->rc : u->lc) = succ = w->rc;//��u������xʱ����Ȼ��ʱ��u�����Ӿ���w����w���Һ������游���ӵ�һ�𣬲�ɾ���˺��w
	}
	hot = w->parent;
	if (succ) succ->parent = hot;
	release(w->data); release(w);
	return succ;
}