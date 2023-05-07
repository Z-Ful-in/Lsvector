#include"Fib.h"
#include<algorithm>
typedef int Rank;
#define DEFAULT_CAPACITY  10
template<typename T>class Vector {
protected:
	Rank _size; Rank _capacity; T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi); //起泡排序算法
	Rank maxItem(Rank lo, Rank hi); //选取最大元素
	void selectionSort(Rank lo, Rank hi); //选择排序算法
	void merge(Rank lo, Rank mi, Rank hi); //归并算法
	void mergeSort(Rank lo, Rank hi); //归并排序算法
	void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
	Rank partition(Rank lo, Rank hi); //轴点构造算法
	void quickSort(Rank lo, Rank hi); //快速排序算法
	void shellSort(Rank lo, Rank hi); //希尔排序算法public:
	// 构造函数
public:
	Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	} //s<=c		
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制				
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
	// 析构函数
	~Vector() { delete[] _elem; } //释放内部空间
	// 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return !_size; } //判空
	int disordered()const;
	Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
	Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
	Rank search(T const& e) const //有序向量整体查找
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
	// 可写访问接口
	T& operator[] (Rank r)const; //重载下标操作符，可以类似于数组形式引用各元素 
	Vector<T>& operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
	T remove(Rank r); //删除秩为r的元素
	int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
	Rank insert(Rank r, T const& e); //插入元素
	Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
	void sort(Rank lo, Rank hi); //对[lo, hi)排序
	void sort() { sort(0, _size); } //整体排序
	void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
	void unsort() { unsort(0, _size); } //整体置乱
	Rank deduplicate(); //无序去重
	Rank uniquify(); //有序去重
	// 遍历
	void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
	template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
}; //Vector
//复制构造函数
template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}
//重载赋值运算符

template<typename T>Vector<T>& Vector<T>::operator=(Vector<T>const& V) {
	if (_elem)delete[]_elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}
//扩容
template<typename T>void Vector<T>::expand() {
	if (_size < _capacity)return;
	if (_capacity < DEFAULT_CAPACITY)_capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[]oldElem;
}
//缩容
template<typename T>void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)return;
	if (_size << 2 > _capacity)return;
	T* oldElem = _elem; _elem = new T[_capacity >> 1];
	for (int i = 0; i < _size; i++)_elem[i] = oldElem[i];
	delete[]oldElem;
}
//重载访问标志
template<typename T>T& Vector<T>::operator[](Rank r) const { return _elem[r]; }
//无序查找
template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi)const {
	while ((lo < hi--) && (e!=_elem[hi]));
	return hi;
}
//无序插入
template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (int i = _size; i > r; i--)_elem[i] = _elem[i - 1];//所有r之后元素向后移一位
	_elem[r] = e; _size++;
	return r;
}
//无序删除
template<typename T>
Rank Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi)return 0;
	while (hi < _size)_elem[lo++] = _elem[hi++];
	_size = lo;//更新尾部，此时的lo已经是删除后的最后端
	shrink();
	return hi - lo;
}
//无序下单元素删除
template<typename T>
T Vector<T>::remove(Rank r) {
	T e = _elem[r];//备份删除元素
	remove(r, r + 1);
	return e;//返回被删除元素 
}
//无序下唯一化
template<typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);//在0-i间搜寻elem[i]的存在问题
	return oldSize - _size;
}

/*void traverse(void (*) (T&)); //遍历（使用函数指针，只读或局部性修改）
	template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）*/
//无序下遍历
template<typename T> void Vector<T>::traverse(void(*visit)(T&)) { for (int i = 0; i < _size; i++)visit(_elem[i]); }
template<typename T>template<typename VST>void Vector<T>::traverse(VST& visit) { for (int i = 0; i < _size; i++)visit(_elem[i]); }
//遍历的应用
template<typename T>struct Increase { virtual void operator()(T& e) { e++; } };
template<typename T>void increase(Vector<T>& V) { V.traverse(Increase<T>()); }//统一递增向量中的各元素，以Increase<T>为基本操作进行遍历

//有序性甄别
template<typename T>int Vector<T>::disordered()const {
	int n = 0;
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i])n++;
	return n;
}
//有序唯一化
template<typename T>int Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j < _size)//此处j不断递增
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];//构造了两个向量i,j，不相等时将j中值不断赋给i,相等时跳过
	_size = ++i; shrink();//截去i后所有重复元素
	return j - i;//删除元素总数
}
//有序二分查找
template<typename T>static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi){
		Rank mi = (lo + hi) >> 1 //二分
			(e < A[mi]) ? hi = mi : lo = mi + 1;
}
return --lo;
}
//斐波那契查找
template<typename T>
static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi) {
	Fib fib(hi - lo);
	while (lo < hi) {
		while (hi - lo < fib.get())fib.prev();
		Rank mi = lo + fib.get() - 1;
		if (e < A[mi])hi = mi;
		else if (A[mi] < e)lo = mi + 1;
		else return mi;
	}
	return -1;
}
//起泡排序
template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (!bubble(lo, hi--));
}
//扫描交换
template<typename T>bool Vector<T>::bubble(Rank lo, Rank hi) {
	bool sorted = true;
	while (++lo < hi) {
		if (_elem[lo - 1] > _elem[lo]) {
			sorted = false;
			swap(_elem[lo - 1], _elem[lo]);
		}
	}
		return sorted;
}
//归并排序
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2)return ;
	int mi = (hi + lo) / 2;
	mergeSort(lo, mi); mergeSort(mi, hi);
	merge(lo, mi, hi);
}
//二路归并
template<typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo;
	int x = mi - lo; T* B = new T[x];
	for (Rank i = 0; i < x; B[i] = A[i++]);
	int y = hi - mi; T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; (j < x) || (k < y);) {
		if ((j < x) && (!(k < y) || (B[j] < C[k])))A[i++] = B[j++];
		if ((k < y) && (!(j < x) || (C[k] < B[j])))A[i++] = C[k++];
	}
	delete[]B;
}