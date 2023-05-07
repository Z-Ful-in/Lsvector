#define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
#define stature(p) ((p) ? (p)->height: -1) //�ڵ�߶�

//BinNode״̬�����ʵ��ж�
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x)==(x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x)==(x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))	//����ӵ��һ������
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))	//ͬʱӵ����������
#define IsLeaf(x) (!HasChild(x))

#define sibling(p) (IsLChild(*(p))?(p)->parent->rc:(p)->parent->lc)	//�ֵ�
#define uncle(x) sibling((x)->parent)	//����

#define FromParentTo(x)  ( IsRoot(x) ? this->_root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) )         //���Ը��׵�����


typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ

template <typename T> struct BinNode {
	BinNodePosi(T) parent;
	BinNodePosi(T) lc;
	BinNodePosi(T) rc; //���ס�����
	int npl;
	RBColor color;
	T data; int height;  //�߶ȡ�������ģ
	//���캯��
	BinNode() :
		parent(nullptr), lc(nullptr), rc(nullptr), height(0), npl(1), color(RB_RED) {}
	BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(1), color(c) {}
	//�����ӿ�
	int size();
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ���Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ�Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //��������������£���ǰ�ڵ��ֱ�Ӻ��
	template <typename VST> void travLevel(VST&); //������α���
	template <typename VST> void travPre(VST&); //�����������
	template <typename VST> void travIn(VST&); //�����������
	template <typename VST> void travPost(VST&); //�����������
};

//��Ϊ���Ӳ���
template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this);
}
//��Ϊ�Һ��Ӳ���
template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{
	return rc = new BinNode(e, this);
}

//size()
template <typename T> int BinNode<T>::size() {//�������������Ϊ���������Ĺ�ģ
	int s = 1; //���뱾��
	if (lc) s += lc->size(); //�ݹ������������ģ
	if (rc) s += rc->size(); //�ݹ������������ģ
	return s;
}//O(n=|size|)


template <typename T>
BinNodePosi(T) BinNode<T>::succ() {//��λ�ڵ�V��ֱ�Ӻ��
	BinNodePosi(T) s = this;
	if (rc) { //�����Һ��ӣ���
		s = rc; //ֱ�Ӻ�̱����������е�
		while (HasLChild(*s))
			s = s->lc; //��С�ڵ�
	}
	else { //������Ӧ�ǡ��Ե�ǰ�ڵ�Ϊֱ��ǰ���ߡ�
		while (IsRChild(*s)) {
			s = s->parent; //���ϳ������ƶ�
		}
		s = s->parent; //����ٳ������ƶ�һ��������NULL
	}
	return s; //��������£�����ʱ��ֱ�Ϊ
} //��ǰ�ڵ�ĸ߶Ȳ���ȣ���O(h)

