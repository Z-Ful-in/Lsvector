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
	void bubbleSort(Rank lo, Rank hi); //���������㷨
	Rank maxItem(Rank lo, Rank hi); //ѡȡ���Ԫ��
	void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
	Rank partition(Rank lo, Rank hi); //��㹹���㷨
	void quickSort(Rank lo, Rank hi); //���������㷨
	void shellSort(Rank lo, Rank hi); //ϣ�������㷨public:
	// ���캯��
public:
	Vector(int c = DEFAULT_CAPACITY, Rank s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	} //s<=c		
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��				
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
	// ��������
	~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
	// ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return !_size; } //�п�
	int disordered()const;
	Rank find(T const& e) const { return find(e, 0, _size); } //���������������
	Rank find(T const& e, Rank lo, Rank hi) const; //���������������
	Rank search(T const& e) const //���������������
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const; //���������������
	// ��д���ʽӿ�
	T& operator[] (Rank r)const; //�����±������������������������ʽ���ø�Ԫ�� 
	Vector<T>& operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	T remove(Rank r); //ɾ����Ϊr��Ԫ��
	int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
	Rank insert(Rank r, T const& e); //����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
	void sort(Rank lo, Rank hi); //��[lo, hi)����
	void sort() { sort(0, _size); } //��������
	void unsort(Rank lo, Rank hi); //��[lo, hi)����
	void unsort() { unsort(0, _size); } //��������
	Rank deduplicate(); //����ȥ��
	Rank uniquify(); //����ȥ��
	// ����
	void traverse(void (*) (T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�
}; //Vector
//���ƹ��캯��
template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}
//���ظ�ֵ�����

template<typename T>Vector<T>& Vector<T>::operator=(Vector<T>const& V) {
	if (_elem)delete[]_elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}
//����
template<typename T>void Vector<T>::expand() {
	if (_size < _capacity)return;
	if (_capacity < DEFAULT_CAPACITY)_capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[]oldElem;
}
//����
template<typename T>void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)return;
	if (_size << 2 > _capacity)return;
	T* oldElem = _elem; _elem = new T[_capacity >> 1];
	for (int i = 0; i < _size; i++)_elem[i] = oldElem[i];
	delete[]oldElem;
}
//���ط��ʱ�־
template<typename T>T& Vector<T>::operator[](Rank r) const { return _elem[r]; }
//�������
template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi)const {
	while ((lo < hi--) && (e!=_elem[hi]));
	return hi;
}
//�������
template<typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (int i = _size; i > r; i--)_elem[i] = _elem[i - 1];//����r֮��Ԫ�������һλ
	_elem[r] = e; _size++;
	return r;
}
//����ɾ��
template<typename T>
Rank Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi)return 0;
	while (hi < _size)_elem[lo++] = _elem[hi++];
	_size = lo;//����β������ʱ��lo�Ѿ���ɾ���������
	shrink();
	return hi - lo;
}
//�����µ�Ԫ��ɾ��
template<typename T>
T Vector<T>::remove(Rank r) {
	T e = _elem[r];//����ɾ��Ԫ��
	remove(r, r + 1);
	return e;//���ر�ɾ��Ԫ�� 
}
//������Ψһ��
template<typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
		(find(_elem[i], 0, i) < 0) ? i++ : remove(i);//��0-i����Ѱelem[i]�Ĵ�������
	return oldSize - _size;
}

/*void traverse(void (*) (T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�*/
//�����±���
template<typename T> void Vector<T>::traverse(void(*visit)(T&)) { for (int i = 0; i < _size; i++)visit(_elem[i]); }
template<typename T>template<typename VST>void Vector<T>::traverse(VST& visit) { for (int i = 0; i < _size; i++)visit(_elem[i]); }
//������Ӧ��
template<typename T>struct Increase { virtual void operator()(T& e) { e++; } };
template<typename T>void increase(Vector<T>& V) { V.traverse(Increase<T>()); }//ͳһ���������еĸ�Ԫ�أ���Increase<T>Ϊ�����������б���

//���������
template<typename T>int Vector<T>::disordered()const {
	int n = 0;
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i])n++;
	return n;
}
//����Ψһ��
template<typename T>int Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j < _size)//�˴�j���ϵ���
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];//��������������i,j�������ʱ��j��ֵ���ϸ���i,���ʱ����
	_size = ++i; shrink();//��ȥi�������ظ�Ԫ��
	return j - i;//ɾ��Ԫ������
}
//������ֲ���
template<typename T>static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
	while (lo < hi){
		Rank mi = (lo + hi) >> 1 //����
			(e < A[mi]) ? hi = mi : lo = mi + 1;
}
return --lo;
}
//쳲���������
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
//��������
template<typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (!bubble(lo, hi--));
}
//ɨ�轻��
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
//�鲢����
template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2)return ;
	int mi = (hi + lo) / 2;
	mergeSort(lo, mi); mergeSort(mi, hi);
	merge(lo, mi, hi);
}
//��·�鲢
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