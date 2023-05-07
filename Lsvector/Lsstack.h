#include"Lsvector.h"
template<typename T>class Stack :public Vector<T> {
public:
	void push(T const& e) { insert(size(), e); }
	T pop{ return remove(size() - 1); }
	T& top() { return(*this)[size() - 1]; }
};
//逆序输出
//进制转换
void convert(Stack<char>& S, _int64 n, int base){
	static char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	if (0 < n) {
		S.push(digit[n % base]);
		convert(S, n / base, base);
	}
}
