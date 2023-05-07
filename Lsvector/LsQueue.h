#include"LsList.h"
#include<iostream>
using namespace std;

template <typename T> class Queue : public List<T> { //���б������Ķ���ģ����
public: //size()��empty()ֱ������
    void enqueue(T const& e) { List<T>::insertAsLast(e); } //���
    T dequeue() { return List<T>::remove(List<T>::first()); } //����
    T& front() { return (List<T>::first())->data; } //����
}; //���б���/ĩ��Ϊ����ͷ/β�����ߵ������أ�



