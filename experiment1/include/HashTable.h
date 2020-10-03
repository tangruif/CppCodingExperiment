// 哈希表桶的链表
typedef struct stringLink {
	char* str;
	struct stringLink* next;
}StringLink;

// 哈希表桶
typedef struct hashBucket {
	StringLink* link;
}HashBucket;

// 字符串哈希表，标识一个字符串是否存在
class HashTable {
public:
	HashTable();

	~HashTable();

	bool hasString(char* str);

	bool addString(char* str);

private:
	// 哈希表桶数组
	HashBucket* hashTable;

	// 哈希表桶数组大小
	const int BUCKET_NUM = 218357;

	// 返回字符串对应在哈希表中的桶
	HashBucket* getBucket(char* str);

	// 返回字符串对应的哈希表中相应节点
	StringLink* getNode(char* str);

	unsigned int ELFHash(char* str);
};

