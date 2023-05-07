#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->height: -1) //节点高度

//BinNode状态与性质的判断
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x)==(x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x)==(x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))	//至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))	//同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))

#define sibling(p) (IsLChild(*(p))?(p)->parent->rc:(p)->parent->lc)	//兄弟
#define uncle(x) sibling((x)->parent)	//叔叔

#define FromParentTo(x)  ( IsRoot(x) ? this->_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )         //来自父亲的引用


typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色

template <typename T> struct BinNode {
	BinNodePosi(T) parent;
	BinNodePosi(T) lc;
	BinNodePosi(T) rc; //父亲、孩子
	int npl;
	RBColor color;
	T data; int height;  //高度、子树规模
	//构造函数
	BinNode() :
		parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(1), color(c) {}
	//操作接口
	int size();
	BinNodePosi(T) insertAsLC(T const&); //作为左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); //作为右孩子插入新节点
	BinNodePosi(T) succ(); //（中序遍历意义下）当前节点的直接后继
	template <typename VST> void travLevel(VST&); //子树层次遍历
	template <typename VST> void travPre(VST&); //子树先序遍历
	template <typename VST> void travIn(VST&); //子树中序遍历
	template <typename VST> void travPost(VST&); //子树后序遍历
};

//作为左孩子插入
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this);
}
//作为右孩子插入
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	return rc = new BinNode(e, this);
}

//size()
template <typename T> int BinNode<T>::size() {//后代总数，以其为根的子树的规模
	int s = 1; //计入本身
	if (lc) s += lc->size(); //递归计入左子树规模
	if (rc) s += rc->size(); //递归计入右子树规模
	return s;
}//O(n=|size|)


template <typename T>
BinNodePosi(T) BinNode<T>::succ() {//定位节点V的直接后继
	BinNodePosi(T) s = this;
	if (rc) { //若有右孩子，则
		s = rc; //直接后继必是右子树中的
		while (HasLChild(*s))
			s = s->lc; //最小节点
	}
	else { //否则后继应是“以当前节点为直接前驱者”
		while (IsRChild(*s)) {
			s = s->parent; //不断朝左上移动
		}
		s = s->parent; //最后再朝右上移动一步可能是NULL
	}
	return s; //两种情况下，运行时间分别为
} //当前节点的高度不深度，过O(h)

