#include "LsBinNode.h"
#include "Lsrelease.h"
#include <iostream>
#include <memory>
#include "LsQueue.h"

#define max(a,b) ( a > b ? a : b)

template <typename T> class BinTree {
protected:
	int _size;//��ģ

	virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x); //����x�����ȵĸ߶�
public:
	BinNodePosi(T) _root; //���ڵ�
	//���캯��
	BinTree() :_size(0), _root(nullptr) {}
	//~BinTree(){ if ( 0 < _size ) remove( _root );}

	//�����ӿ�
	int size() const { return _size; } //��ģ
	bool empty() const { return !_root; } //�п�
	BinNodePosi(T) root() const { return _root; } //����

	BinNodePosi(T) insertAsRoot(T const& e); //��Ϊ���ڵ����
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);//��Ϊ���ӽڵ����
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);//��Ϊ�Һ��ӽڵ����
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& S);//��Ϊ����������
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& S);//��Ϊ����������

	int remove(BinNodePosi(T) x);//ɾ���Խڵ�xΪ��������

	BinTree<T>* secede(BinNodePosi(T) x);//�������ӵ�ǰ����ժ����������ת��Ϊһ�Ŷ�������
	template <typename VST> void travPre(BinNodePosi(T) x, VST& visit) { if (_root) _root->travPre(visit); }//�������
	template <typename VST> void travPost(const VST& visit) { if (_root) _root->travPost(visit); }//�������
	template <typename VST> void travLevel(const VST& visit) { if (_root) _root->travLevel(visit); }//��α���
	template <typename VST> void travIn(const VST& visit) { if (_root) _root->travIn(visit); }//�������

	void traverse(BinNodePosi(T) x, void (*)(T&));

};
template <typename T> void BinTree<T>::traverse(BinNodePosi(T) x, void (*visit)(T&))
{
	if (!x) return;
	visit(x->data);
	traverse(x->lc, visit);

	traverse(x->rc, visit);

}
//�������
template <typename T, typename VST> void travPre(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	//BinNodePosi(T) x = root();
	visit(x->data);
	travPre(x->lc, visit);
	travPre(x->rc, visit);

}
//�������
template <typename T, typename VST> void travPost(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travPost(x->lc, visit);
	travPost(x->rc, visit);
	visit(x->data);

}
//�������
template <typename T, typename VST> void travIn(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	travIn(x->lc, visit);
	visit(x->data);
	travIn(x->rc, visit);


}
//��α���
template <typename T> template <typename VST> void BinNode<T>::travLevel(VST& visit) {
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(this);
	while (!Q.empty()) {
		BinNodePosi(T) x = Q.dequeue(); visit(x->data);
		if (HasLChild(*x)) Q.enqueue(x->lc);
		if (HasRChild(*x)) Q.enqueue(x->rc);
	}
}
//���½ڵ�x�ĸ߶�
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

//�������ĸ߶�
template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x) { updateHeight(x); x = x->parent; }
}

//����������ڵ�
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1; return _root = new BinNode<T>(e);
}

//��Ϊ�ڵ�x�����Ӳ���
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsLC(e); updateHeightAbove(x); return x->lc;
}
//��Ϊ�ڵ�x���Һ��Ӳ���
template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++; x->insertAsRC(e); updateHeightAbove(x); return x->rc;
}
//S��Ϊ�ڵ�x������������
//S�����ÿ�
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
//S��Ϊ�ڵ�x������������
//S�����ÿ�
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

template <typename T>//������ɾ��x�ڵ㼰����
int BinTree<T>::remove(BinNodePosi(T) x)
{
	FromParentTo(*x) = nullptr;
	updateHeightAbove(x->parent);
	int n = removeAt(x); _size -= n;
	return n;
}

template <typename T>//ɾ��x�ڵ㼰����������ɾ���ڵ���
static int removeAt(BinNodePosi(T) x)
{
	if (!x)return 0;//�ݹ��
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	dtl::release(x->data); dtl::release(x); return n;
}
template<typename T>BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {//asert Ϊ�������Ϸ�λ��
	FromParentTo(*x) = nullptr;//�ж����Ը��ڵ��ָ��
	updateHeightAbove(x->parent);//����ԭ�����������ȵĸ߶�
	BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr;//������xΪ��
	S->_size = x->size(); _size -= S->_size; return S;//���¹�ģ�����ط������������
}
