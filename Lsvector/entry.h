#ifndef ENTRY_H_INCLUDED
#define ENTRY_H_INCLUDED
template<typename K, typename V> struct Entry
{
	K key;      //�ؼ���
	V value;    //������
	//���캯��
	Entry(K k = K(), V v = V()) :key(k), value(v) {}
	Entry(const Entry<K, V>& e) :key(e.key), value(value) {}
	bool operator<(const Entry<K, V>& e);
	bool operator>(const Entry<K, V>& e);
	bool operator==(const Entry<K, V>& e);
	bool operator!=(const Entry<K, V>& e);
};
#endif // ENTRY_H_INCLUDED
