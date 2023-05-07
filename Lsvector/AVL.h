#pragma once
#include"BST.h"
#define Balanced(x) (stature((x).lc)==stature((x).rc))
#define BalFac(x) (stature((x).lc)-stature((x).rc))
#define AvlBalanced(x) ((-2<BalFac(x)&&BalFac(x)<2))
#define FromParentTo(x) (IsRoot(x)?_root:(IsLChild(x)?(x).parent->lc:(x).parent->rc))
#define tallerChild(x) (\
	stature((x)->lc) > stature((x)->rc)? (x)->lc:( \
	stature((x)->lc) < stature((x)->rc)? (x)->rc:( \
	IsLChild(*(x))? (x)->lc:(x)->rc\
	)\
	)\
	)
template<typename T>class AVL :public BST<T> {
public:BinNodePosi(T) insert(const T&);
	  bool remove(const T&);
};
template<typename T>BinNodePosi(T) AVL<T>::insert(const T& e) {
	BinNodePosi(T)& x = BST<T>::search(e); if (x)return x;
	BinNodePosi(T) xx = x = new BinNode<T>(e, BST<T>::_hot, BST<T>::_size++);
	for (BinNodePosi(T) g = BST<T>::_hot; g; g = g->parent) {
		if (!AvlBalanced(*g)) {
			if (IsRoot(*g)) {
				this->_root = BST<T>::rotateAt((tallerChild(tallerChild(g))));
			}
			else if (g == (g->parent)->lc) {
				(g->parent)->lc = BST<T>::rotateAt((tallerChild(tallerChild(g))));
			}
			else {
				(g->parent)->rc = BST<T>::rotateAt((tallerChild(tallerChild(g))));;
			}break;
		}
			else
				BST<T>::updataHeight(g);
		}
		return xx;
}