#ifndef ENTRY_H_INCLUDED
#define ENTRY_H_INCLUDED
template<typename K, typename V> struct Entry
{
	K key;      //关键码
	V value;    //数据项
	//构造函数
	Entry(K k = K(), V v = V()) :key(k), value(v) {}
	Entry(const Entry<K, V>& e) :key(e.key), value(value) {}
	bool operator<(const Entry<K, V>& e);
	bool operator>(const Entry<K, V>& e);
	bool operator==(const Entry<K, V>& e);
	bool operator!=(const Entry<K, V>& e);
};
#endif // ENTRY_H_INCLUDED
