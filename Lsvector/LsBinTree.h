#include "LsBinNode.h"
#include "Lsrelease.h"
#include <iostream>
#include <memory>
#include "LsQueue.h"

#define max(a,b) ( a > b ? a : b)

template <typename T> class BinTree {
protected:
	int _size;//规模

	virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
	void updateHeightAbove(BinNodePosi(T) x); //更新x及祖先的高度
public:
	BinNodePosi(T) _root; //根节点
	//构造函数
	BinTree() :_size(0), _root(nullptr) {}
	//~BinTree(){ if ( 0 < _size ) remove( _root );}

	//函数接口
	int size() const { return _size; } //规模
	bool empty() const { return !_root; } //判空
	BinNodePosi(T) root() const { return _root; } //树根

	BinNodePosi(T) insertAsRoot(T const& e); //作为根节点插入
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);//作为左孩子节点接入
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);//作为右孩子节点接入
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& S);//作为左子树接入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& S);//作为右子树接入

	int remove(BinNodePosi(T) x);//删除以节点x为根的子树

	BinTree<T>* secede(BinNodePosi(T) x);//将子树从当前树中摘除，并将其转换为一颗独立子树
	template <typename VST> void travPre(BinNodePosi(T) x, VST& visit) { if (_root) _root->travPre(visit); }//先序遍历
	template <typename VST> void travPost(const VST& visit) { if (_root) _root->travPost(visit); }//后序遍历
	template <typename VST> void travLevel(const VST& visit) { if (_root) _root->travLevel(visit); }//层次遍历
	template <typename VST> void travIn(const VST& visit) { if (_root) _root->travIn(visit); }//中序遍历

	void traverse(BinNodePosi(T) x, void (*)(T&));

};
template <typename T> void BinTree<T>::traverse(BinNodePosi(T) x, void (*visit)(T&))
{
	if (!x) return;
	visit(x->data);
	traverse(x->lc, visit);

	traverse(x->rc, visit);

}
//先序遍历
template <typename T, typename VST> void travPre(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	//BinNodePosi(T) x = root();
	visit(x->data);
	travPre(x->lc, visit);
	travPre(x->rc, visit);

}
//后序遍历
template <typename T, typename VST> void travPost(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travPost(x->lc, visit);
	travPost(x->rc, visit);
	visit(x->data);

}
//中序遍历
template <typename T, typename VST> void travIn(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travIn(x->lc, visit);
	visit(x->data);
	travIn(x->rc, visit);


}
//层次遍历
template <typename T> template <typename VST> void BinNode<T>::travLevel(VST& visit) {
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(this);
	while (!Q.empty()) {
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);
		if (HasLChild(*x)) Q.enqueue(x->lc);
		if (HasRChild(*x)) Q.enqueue(x->rc);
	}
}
//更新节点x的高度
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

//更新树的高度
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) { updateHeight(x); x = x->parent; }
}

//空树插入根节点
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1; return _root = new BinNode<T>(e);
}

//作为节点x的左孩子插入
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsLC(e); updateHeightAbove(x); return x->lc;
}
//作为节点x的右孩子插入
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsRC(e); updateHeightAbove(x); return x->rc;
}
//S作为节点x的左子树插入
//S本身被置空
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& S)
{
	x->lc = S->_root;
	x->lc->parent = x;

	_size += S->_size;
	updateHeightAbove(x);
	S->_root = nullptr;
	S->size = 0; dtl::release(S); S = nullptr; return x;
}
//S作为节点x的右子树插入
//S本身被置空
template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& S)
{
	x->rc = S->_root;
	x->rc->parent = x;

	_size += S->_size;
	updateHeightAbove(x);
	S->_root = nullptr;
	S->size = 0; dtl::release(S); S = nullptr; return x;
}

template <typename T>//二叉树删除x节点及其后代
int BinTree<T>::remove(BinNodePosi(T) x)
{
	FromParentTo(*x) = nullptr;
	updateHeightAbove(x->parent);
	int n = removeAt(x); _size -= n;
	return n;
}

template <typename T>//删除x节点及其后代，返回删除节点数
static int removeAt(BinNodePosi(T) x)
{
	if (!x)return 0;//递归基
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	dtl::release(x->data); dtl::release(x); return n;
}
template<typename T>BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {//asert 为二叉树合法位置
	FromParentTo(*x) = nullptr;//切断来自父节点的指针
	updateHeightAbove(x->parent);//更新原树中所有祖先的高度
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr;//新树以x为根
	S->_size = x->size(); _size -= S->_size; return S;//更新规模，返回分离回来的子树
}
