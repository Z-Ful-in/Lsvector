#include"LsList.h"
#include<iostream>
using namespace std;

template <typename T> class Queue : public List<T> { //由列表派生的队列模板类
public: //size()与empty()直接沿用
    void enqueue(T const& e) { List<T>::insertAsLast(e); } //入队
    T dequeue() { return List<T>::remove(List<T>::first()); } //出队
    T& front() { return (List<T>::first())->data; } //队首
}; //以列表首/末端为队列头/尾——颠倒过来呢？



