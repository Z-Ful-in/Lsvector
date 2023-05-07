#include<iostream>
#include"lssstack.h"
#include"LsQueue.h"
using namespace std;
int main() {
	Vector<float> nums(10);
	for (float i = 0; i < 10; i++)
		nums.insert(i, -32.5 * i * i + 66.5 * i + 11);
	cout << nums.size() << endl;
	cout << nums[0] << nums[1] << nums[2] << endl;
	nums.deduplicate();
	Stack<char>S; _int64 n = 101; int base = 2;
	convert(S, n, base);
	while (!S.empty())
		cout << S.pop();
	cout << endl;
	const char exp[10] = { '{','(',')','[','(','{','}',')',']','}' };
	cout << paren(exp, 0, 10) << endl;
	cout << "ÒÔÉÏÎªÀ¨ºÅÆ¥ÅäÅÐ¶Ï" << endl;
	Queue<int> q;
	for (int i = 1; i <= 3; i++) {
		q.enqueue(i);
		int temp = q.front();
		cout << i << " ";
		cout << q.dequeue() << endl;
	}
	char p[10]; cin >> p;
	cout << evaluate(p);
}