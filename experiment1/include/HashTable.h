// ��ϣ��Ͱ������
typedef struct stringLink {
	char* str;
	struct stringLink* next;
}StringLink;

// ��ϣ��Ͱ
typedef struct hashBucket {
	StringLink* link;
}HashBucket;

// �ַ�����ϣ����ʶһ���ַ����Ƿ����
class HashTable {
public:
	HashTable();

	~HashTable();

	bool hasString(char* str);

	bool addString(char* str);

private:
	// ��ϣ��Ͱ����
	HashBucket* hashTable;

	// ��ϣ��Ͱ�����С
	const int BUCKET_NUM = 218357;

	// �����ַ�����Ӧ�ڹ�ϣ���е�Ͱ
	HashBucket* getBucket(char* str);

	// �����ַ�����Ӧ�Ĺ�ϣ������Ӧ�ڵ�
	StringLink* getNode(char* str);

	unsigned int ELFHash(char* str);
};

