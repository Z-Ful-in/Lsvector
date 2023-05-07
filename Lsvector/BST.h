#pragma once
#include"LsBinTree.h"
#include"Lsrelease.h"
template<typename T>class BST :public BinTree<T> {
public:	virtual BinNodePosi(T)& search(const T&);//如果查不到，最终会指向最终节点的下一位空节点，而此时的_hot作为静态函数可以指向空节点的父亲
	virtual BinNodePosi(T) insert(const T&);
	virtual bool remove(const T&);

	static BinNodePosi(T)& searchIn(BinNodePosi(T) &v,const T&e,BinNodePosi(T)_hot) {
		if (!v || (e == v->data))return v;
		_hot = v;
		return searchIn((e < v->data ? v->lc : v->rc), e, _hot);//_hot变量会指向最终节点的末端，是最终节点（无论是否查到）的父亲。
	}
protected:
	BinNodePosi(T) _hot;
	BinNodePosi(T) connect34(BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T));
};

template<typename T>BinNodePosi(T)& BST<T>::search(const T& e) { return searchIn(_root, e, _hot = NULL); }
template<typename T>BinNodePosi(T) BST<T>::insert(const T& e) {
	BinNodePosi(T) &x = search(e);//search的返回值是引用类型，可以作为左值进行封装
	if (!x) {//防止出现雷同情况
		x = new BinNodePosi(e, _hot);
		_size++; updateHeightAbove(x);
	}
	return x;
}
template<typename T>bool BST<T>::remove(T const& e) {
	BinNodePosi(T) &x = search(e);
	if (!x)return false;//如果查找不到，返回空
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
		w = w->succ; swap(x->data, w->data);//互换x与其后继（中序意义）节点的数据，此时的w最多只有一个右孩子（比它大）
		BinNodePosi(T)u = w->parant;//找到w的父亲，即（右子树存在的话）右孩子的祖父
		(u == x ? u->rc : u->lc) = succ = w->rc;//当u不等于x时，显然此时的u的左孩子就是w，将w的右孩子与祖父链接到一起，并删除此后的w
	}
	hot = w->parent;
	if (succ) succ->parent = hot;
	release(w->data); release(w);
	return succ;
}