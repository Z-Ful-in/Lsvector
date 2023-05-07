
typedef int Rank;//秩
#define ListNodePosi(T) ListNode<T>*//列表节点位置

template <typename T> struct ListNode {//列表节点模板类
    //成员
    T data; ListNodePosi(T) pred; ListNodePosi(T) succ;//数值，前驱，后继
    ListNode() {};//针对header和trailer的构造
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {};//默认构造器
    //操作接口
    ListNodePosi(T) insertAsPred(T const& e);//紧靠当前节点之前插入节点
    ListNodePosi(T) insertAsSucc(T const& e);//仅随当前节点之后插入新节点
};
template <typename T> class List {//List模板类
private:
    int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;//规模，头哨兵，尾哨兵
protected:
    void init();//列表创建时的初始化
    int clear();//清除所有节点
    void copyNodes(ListNodePosi(T), int);//复制列表中自位置p启的n项
    void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m);//归并
    void mergeSort(ListNodePosi(T)& p, int n);//对p连续的n个节点归并排序
    void selectionSort(ListNodePosi(T) p, int n);//对从p点开始连续的n个因子排序
    void insertionSort(ListNodePosi(T) p, int n);//对从p点开始的n个因子插入排序
public:
    //构造函数
    List() { init(); };//默认
    List(List<T>const& L);//整体复制列表L
    List(List<T>const& L, Rank r, Rank n);//复制列表L中第r项起的n项
    List(ListNodePosi(T) p, int n);//复制列表中自位置p起的第n项
    //析构函数
    ~List();//释放(包含头，尾哨兵在内的)所有节点
    //只读访问接口
    Rank size() const { return _size; }//规模
    bool empty() const { return _size <= 0; }//判空
    T& operator[](Rank r)const;//重载，支持寻秩访问(效率低)
    ListNodePosi(T)first()const { return header->succ; };//首节点位置
    ListNodePosi(T) last()const { return trailer->pred; }//末节点位置
    bool valid(ListNodePosi(T) p)//判断位置p是否对外合法
    {
        return p && (trailer != p) && (header != p);
    }//将头尾节点等同于空}
    int disordered() const;//判断列表是否已排序
    ListNodePosi(T) find(T const& e)const//无序列表查找
    {
        return find(e, _size, trailer);
    };
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p)const;//无序区间查找
    ListNodePosi(T) search(T const& e)//有序列表查找
    {
        return search(e, _size, trailer);
    }
    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p)const;//有序区间查找
    ListNodePosi(T) selectMax(ListNodePosi(T)p, int n);//在p及其后面的n-1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); }//整体最大者
    //可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);//将e当做首节点插入
    ListNodePosi(T) insertAsLast(T const& e);//将e当做末节点插入
    ListNodePosi(T) insertA(ListNodePosi(T)p, T const& e);//将e当做p的后继插入
    ListNodePosi(T) insertB(ListNodePosi(T)p, T const& e);//将e当做p的前驱插入
    T remove(ListNodePosi(T) p);//删除合法位置p处的节点，返回被删除的节点
    void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); }//全列表归并
    void sort(ListNodePosi(T) p, int n);//列表区间排序
    void sort() { sort(first(), _size); }//列表整体排序
    int deduplicate();//无序去重
    int uniquify();//有序去重
    void reverse();//前后倒置
    //遍历
    void traverse(void (*)(T&));//遍历，依次实施visit操作(函数指针，只读或者局部性修改)
    template <typename VST>//操作器
    void traverse(VST&);//遍历，依次实施visit操作(函数对象，可全局性修改)
};//List
template <typename T> void List<T>::init() {//列表初始化，创建列表对象时统一调用
    header = new ListNode<T>;//创建头哨兵节点
    trailer = new ListNode<T>;//创建尾哨兵节点
    header->succ = trailer; header->pred = NULL;
    trailer->pred = header; trailer->succ = NULL;
    _size = 0;//记录规模
}
template <typename T>//重载下标操作符号，以便通过秩直接访问列表节点(虽方便但效率低，需慎用)
T& List<T>::operator[](Rank r)const {
    ListNodePosi(T) p = first();//从首节点出发
    while (0 < r--)p = p->succ;//顺数第r个节点即是
    return p->data;//目标节点，返回其中所存元素
}
template <typename T>//在无序列表内节点p(可能是trailer)的n个(真)前驱中，找到等于e的最后者
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p)const {
    while (0 < n--) {//(0<=n<=Rank(p)<_size)对于p的n个前驱从左向右
        if (e == (p = p->pred)->data) return p;//逐个对比直至命中或者越界
        return NULL;//p越过左边界意味区间内不含e，查找失败
    }//查找失败时返回NULL
}
template <typename T> ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
    _size++; return header->insertAsSucc(e);
}//把e当做首节点插入

template <typename T> ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
    _size++; return trailer->insertAsPred(e);
}//把e当做末节点插入

template <typename T> ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e)
{
    _size++; return p->insertAsSucc(e);
}//把e当做p的后继节点插入

template <typename T> ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e)
{
    _size++; return p->insertAsPred(e);
}//把e当做p的前驱节点插入
template <typename T> //前插入算法，将新元素e当做节点的前驱
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) { //O(1)
    ListNodePosi(T) x = new ListNode(e, pred, this); //创建（耗时100倍）
    pred->succ = x; pred = x; //次序不可颠倒
    return x; //建立链接，返回新节点的位置
} //得益于哨兵，即便this为首节点亦不必特殊处理——此时等效于insertAsFirst(e)
template <typename T> //后插入算法，将新元素e当做节点的后继
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) { //O(1)
    ListNodePosi(T) x = new ListNode(e, this, succ); //创建（耗时100倍）
    succ->pred = x; succ = x; //设置逆向链接
    return x; //返回新节点的位置
}
template <typename T> void List<T>::copyNodes(ListNodePosi(T) p, int n) { //O(n)
    init(); //创建头、尾哨兵节点并做初始化
    while (n--) { //将起自p的n项依次作为末节点
        insertAsLast(p->data); //插入
        p = p->succ;
    }
}  template<typename T>//整体列表中自p起n项
List<T>::List(ListNodePosi(T) p, int n) { copyNodes(p, n); }

template<typename T>//整体复制列表L
List<T>::List(List<T> const& L) { copyNodes(L.first(), L._size); }

template<typename T>//复制L中自r项起的第n项
List<T>::List(List<T> const& L, int r, int n) { copyNodes(L[r], L._size); }

template <typename T> //删除合法位置p处节点，返回其数值
T List<T>::remove(ListNodePosi(T) p) { //O(1)
    T e = p->data; //备份待删除节点数值（设类型T可直接赋值）
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p; _size--; return e; //返回备份数值
}
template <typename T> List<T>::~List() //列表析构
{
    clear(); delete header; delete trailer;
} //清空列表，释放头、尾哨兵节点

template <typename T> int List<T>::clear() { //清空列表，O(n)
    int oldSize = _size;
    while (0 < _size) //反复删除首节点
        remove(header->succ);
    return oldSize;
}
template <typename T> int List<T>::deduplicate() {
    int oldSize = _size;
    ListNodePosi(T) p = first();
    for (Rank r = 0; p != trailer; p = p->succ) //O(n)
        if (ListNodePosi(T) q = find(p->data, r, p))
            remove(q);
        else
            r++; //无重前缀的长度
    return oldSize - _size; //删除元素总数
} //正确性及效率分析的方法与结论，与Vector::deduplicate()相同
template <typename T>
void List<T>::traverse(void (*visit)(T&)) //形参是函数指针，无返回值，这时我们可以直接把函数当成实参使用，此时它会自动转换成指针
{
    ListNodePosi(T) p = header; while ((p = p->succ) != trailer) visit(p->data);//visit是我们自己定义的函数，可以作为traverse的形参放入并实现访问
}

template <typename T> template <typename VST>
void List<T>::traverse(VST& visit) //函数对象
{
    ListNodePosi(T) p = header; while ((p = p->succ) != trailer) visit(p->data);
}
template <typename T> int List<T>::uniquify() { //剔除重复元素
    if (_size < 2) return 0; //平凡列表自然无重复
    int oldSize = _size; //记录原规模
    ListNodePosi(T) p = first(); ListNodePosi(T) q; //各区段起点及其直接后继
    while (trailer != (q = p->succ)) //反复考查紧邻的节点对(p,q)
        if (p->data != q->data) p = q; //若互异，则转向下一对
        else remove(q); //否则（雷同），删除后者
    return oldSize - _size; //规模变化量，即被删除元素总数
} //只需遍历整个列表一趟，O(n)
//在有序列表内节点p的n个（真）前驱中，找到不大于e的最靠后者
template <typename T> // assert: 0 <= n <= rank(p) < _size
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
    do { p = p->pred; n--; } //从右向左
    while ((-1 < n) && (e < p->data)); //逐个比较，直至命中或越界
    return p;
}//失败时，返回区间左边界的前驱（可能是header）
template <typename T> void List<T>::sort(ListNodePosi(T) p, int n) {//列表区间排序
    switch (rand() % 3) {//随机选取排序算法，可根据具体问题特点扩充
    case 1: insertionSort(p, n); break;//插入排序
    case 2: selectionSort(p, n); break;//选择排序
    case 0: mergeSort(p, n); break;//归并排序
    }
}
//对列表中起始于位置p、宽度为n的区间做选择排序，valid(p) && rank(p) + n <= size
template <typename T> void List<T>::selectionSort(ListNodePosi(T) p, int n) {
    ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
    for (int i = 0; i < n; i++) tail = tail->succ; //待排序区间为(head, tail)
    while (1 < n) { //反复从（非平凡）待排序区间内找出最大者，并移至有序区间前端
        insertB(tail, remove(selectMax(head->succ, n))); //改进...
        tail = tail->pred; n--; //待排序区间、有序区间的范围，均同步更新
    }
}
template <typename T> //从起始于位置p的n个元素中选出最大者，1 < n
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) { //(n)
    ListNodePosi(T) max = p; //最大者暂定为p
    for (ListNodePosi(T) cur = p; 1 < n; n--) //后续节点逐一与max比较
        if (!lt((cur = cur->succ)->data, max->data)) //data  max
            max = cur; //则更新最大元素位置记录
    return max; //返回最大节点位置
}
//对列表中起始于位置p、宽度为n的区间做插入排序，valid(p) && rank(p) + n <= size
template <typename T> void List<T>::insertionSort(ListNodePosi(T) p, int n) {
    for (int r = 0; r < n; r++) { //逐一引入各节点，由Sr得到Sr+1
        insertA(search(p->data, r, p), p->data); //查找 + 插入
        p = p->succ; remove(p->pred); //转向下一节点
    } //n次迭代，每次O(r + 1)
} //仅使用O(1)辅助空间，属于就地算法
template <typename T> //自p起的n个元素，与L中自q起的m个元素归并（归并排序时L==this）
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
    ListNodePosi(T) pp = p->pred; //借助前驱（可能是header），以便返回前 ...
    while (0 < m) //在q尚未移出区间之前
        if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p) <= v(q)
        {
            if (q == (p = p->succ)) break; n--;
        } //则将p直接后移
        else //若p已超出右界或v(q) < v(p)，则将q插至p之前
        {
            insertB(p, L.remove((q = q->succ)->pred)); m--;
        }
    p = pp->succ; //确定归并后区间的（新）起点
} //运行时间O(n + m)，线性正比于节点总数

template <typename T> //valid(p) && rank(p) + n <= size
void List<T>::mergeSort(ListNodePosi(T)& p, int n) { //对起始于位置p的n个元素排序
    if (n < 2) return; //待排序范围足够小时直接返回，否则...
    ListNodePosi(T) q = p; int m = n >> 1; //以中点为界
    for (int i = 0; i < m; i++) q = q->succ; //均分列表：O(m) = O(n)
    mergeSort(p, m); mergeSort(q, n - m); //子序列分别排序
    merge(p, m, *this, q, n - m); //归并
} //若归并可在线性时间内完成，则总体运行时间亦为O(nlogn)


