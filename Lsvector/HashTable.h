#include "dictionary.h" //����ʵ�ADT
#include "BitMap.h" //����λͼ
#include "entry.h"
template <typename K, typename V> //key��value
class Hashtable : public Dictionary<K, V> { //����Dictionary�ӿڵ�Hashtableģ����
	/*DSA*/friend class UniPrint;
private:
	Entry<K, V>** ht; //Ͱ���飬��Ŵ���ָ��	
	int M; //Ͱ��������
	int N; //��������
	Bitmap* lazyRemoval; //����ɾ�����
#define lazilyRemoved(x)  (lazyRemoval->test(x))
#define markAsRemoved(x)  (lazyRemoval->set(x))
protected:
	int probe4Hit(const K& k); //�عؼ���k��Ӧ�Ĳ��������ҵ�����ƥ���Ͱ
	int probe4Free(const K& k); //�عؼ���k��Ӧ�Ĳ��������ҵ��׸����ÿ�Ͱ
	void rehash(); //��ɢ���㷨������Ͱ���飬��֤װ�������ھ���������
public:
	Hashtable(int c = 5); //����һ��������С��c��ɢ�б�Ϊ������ʱѡ�ý�С��Ĭ��ֵ��
	~Hashtable(); //�ͷ�Ͱ���鼰���и����ǿգ�Ԫ����ָ��Ĵ���
	int size() const { return N; } // ��ǰ�Ĵ�����Ŀ
	bool put(K, V); //���루��ֹ��ͬ�������ʿ���ʧ�ܣ�
	V* get(K k); //��ȡ
	bool remove(K k); //ɾ��
};
template <typename K, typename V> Hashtable<K, V>::Hashtable(int c) { //����ɢ�б�����Ϊ
	//M = primeNLT ( c, 1048576, "prime-1048576-bitmap.txt" ); //��С��c������M
	M = 7;
	N = 0; ht = new Entry<K, V>*[M]; //����Ͱ���飨����˶�����ɹ�������ʼװ������ΪN/M = 0%
	memset(ht, 0, sizeof(Entry<K, V>*) * M); //��ʼ����Ͱ
	lazyRemoval = new Bitmap(M); //����ɾ����Ǳ���ͼ
	//*DSA*/printf("A bucket array has been created with capacity = %d\n\n", M);
}
//����
template <typename K, typename V> Hashtable<K, V>::~Hashtable() { //����ǰ�ͷ�Ͱ���鼰�ǿմ���
	for (int i = 0; i < M; i++) //��һ����Ͱ
		if (ht[i]) dtl::release(ht[i]); //�ͷŷǿյ�Ͱ
	dtl::release(ht); //�ͷ�Ͱ����
	dtl::release(lazyRemoval); //�ͷ�����ɾ�����
}
//����
template <typename K, typename V> V* Hashtable<K, V>::get(K k) //ɢ�б���������㷨
{
	int r = probe4Hit(k); return ht[r] ? &(ht[r]->value) : NULL;
} //��ֹ������keyֵ��ͬ
template <typename K, typename V> int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(k) % M; //����ʼͰ�������෨ȷ��������
	//*DSA*/printf(" ->%d", r);
	while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r)))
		r = (r + 1) % M; //�ز�����������̽���������г�ͻ��Ͱ���Լ�������ɾ����ǵ�Ͱ
	//*DSA*/printf(" ->%d", r);
	//*DSA*/printf("\n");
	return r; //�����߸���ht[r]�Ƿ�Ϊ�գ������жϲ����Ƿ�ɹ�
}
//ɾ��
template <typename K, typename V> bool Hashtable<K, V>::remove(K k) { //ɢ�б����ɾ���㷨
	int r = probe4Hit(k); if (!ht[r]) return false; //��Ӧ����������ʱ���޷�ɾ��
	dtl::release(ht[r]); ht[r] = NULL; markAsRemoved(r); N--; return true;
	//�����ͷ�Ͱ�д�������������ɾ����ǣ������´�������
}
//����
template <typename K, typename V> bool Hashtable<K, V>::put(K k, V v) { //ɢ�б��������
	if (ht[probe4Hit(k)]) return false; //��ͬԪ�ز����ظ�����
	int r = probe4Free(k); //Ϊ�´����Ҹ���Ͱ��ֻҪװ�����ӿ��Ƶõ�����Ȼ�ɹ���
	ht[r] = new Entry<K, V>(k, v); ++N; //���루ע�⣺����ɾ��������踴λ��
	if (N * 2 > M) rehash(); //װ�����Ӹ���50%����ɢ��
	return true;
}
template <typename K, typename V> int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M; //����ʼͰ�������෨ȷ��������
	//*DSA*/printf(" ->%d", r); //�׸���̽��Ͱ��Ԫ��ַ
	while (ht[r]) r = (r + 1) % M; //�ز�������Ͱ��̽��ֱ���׸���Ͱ�������Ƿ��������ɾ����ǣ�
	//*DSA*/   while (ht[r]) { r = (r+1) % M; printf(" ->%d", r); } printf("\n");
	return r; //Ϊ��֤��Ͱ�����ҵ���װ�����Ӽ�ɢ�б���Ҫ��������
}
//��ɢ��
/******************************************************************************************
 * ��ɢ���㷨��װ�����ӹ���ʱ����ȡ����һȡ���ٲ��롱�����ز��ԣ���Ͱ��������
 * ���ɼ򵥵أ�ͨ��memcpy()����ԭͰ���鸴�Ƶ���Ͱ���飨����ǰ�ˣ�����������������⣺
 * 1����̳�ԭ�г�ͻ��2�����ܵ��²������ں�˶��ѡ�������Ϊ��������Ͱ��������ɾ����־Ҳ�޼�����
 ******************************************************************************************/
template <typename K, typename V> void Hashtable<K, V>::rehash() {
	int old_capacity = M; Entry<K, V>** old_ht = ht;
	//M = primeNLT ( 2 * M, 1048576, "prime-1048576-bitmap.txt" ); //�������ټӱ�
	M = 17;
	N = 0; ht = new Entry<K, V>*[M]; memset(ht, 0, sizeof(Entry<K, V>*) * M); //��Ͱ����
	dtl::release(lazyRemoval); lazyRemoval = new Bitmap(M); //�¿�����ɾ����Ǳ���ͼ
	//*DSA*/printf("A bucket array has been created with capacity = %d\n\n", M);
	for (int i = 0; i < old_capacity; i++) //ɨ��ԭͰ����
		if (old_ht[i]) //���ǿ�Ͱ�еĴ�����һ
			put(old_ht[i]->key, old_ht[i]->value); //�������µ�Ͱ����
	dtl::release(old_ht); //�ͷ�ԭͰ���顪����������ԭ�ȴ�ŵĴ�������ת�ƣ���ֻ���ͷ�Ͱ���鱾��
}
static size_t hashCode(char c) { return (size_t)c; } //�ַ�
static size_t hashCode(int k) { return (size_t)k; } //�����Լ���������
static size_t hashCode(long long i) { return (size_t)((i >> 32) + (int)i); }
static size_t hashCode(char s[]) { //�����ַ�����ѭ����λɢ���루cyclic shift hash code��
	int h = 0; //ɢ����
	for (size_t n = strlen(s), i = 0; i < n; i++) //�������ң��������ÿһ�ַ�
	{
		h = (h << 5) | (h >> 27); h += (int)s[i];
	} //ɢ����ѭ������5λ�����ۼӵ�ǰ�ַ�
	return (size_t)h; //������õ�ɢ���룬ʵ���Ͽ����Ϊ���Ƶġ�����ʽɢ���롱
} //����Ӣ�ﵥ�ʣ�"ѭ������5λ"��ʵ��ͳ�Ƶó������ֵ
