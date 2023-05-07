#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

template<typename K, typename V> struct Dictionary
{
	virtual int size() const = 0;//��ǰ��������
	virtual bool put(K, V) = 0;//�������
	virtual V* get(K k) = 0;//��ȡ����
	virtual bool remove(K k) = 0;//ɾ������
};

#endif // DICTIONARY_H_INCLUDED
