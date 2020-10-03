typedef struct BPlusNode {
	int keyNum;
	// ʹ���ַ���ǰ׺4��char��Ϊ�����������Ҷ�ӽڵ㣬��洢���ݣ�
	char** Keys;
	// �ӽڵ�
	struct BPlusNode** child;
	// ���ڵ�
	struct BPlusNode* parent;
	// �����Ҷ�ӽڵ㣬����next
	struct BPlusNode* next;
}BNode;

class BPlusTree {
public:

	BPlusTree(int order);

	~BPlusTree();

	bool addString(char* str);
	
	bool hasString(char* str);

	void outputTree();

	bool removeString(char* str);


private:
	BNode* root = NULL;

	// B+���Ľ�����ÿ���ڵ�����ӽڵ����
	int order;

	BNode* searchNode(char* str, bool need_insert);

	// ������ڵ���нڵ���Ѳ���������ֵΪ�½ڵ�
	// < order / 2�����
	// >= order / 2���ұ�
	// ������ڵ���ѣ��Զ������µĸ��ڵ�
	BNode* devideNode(BNode* node);

	// �����½ڵ�
	BNode* createNewNode(BNode* parent, bool is_leaf);

	// �ڽڵ��е�index��λ�ò����ַ���
	bool insertIntoNode(BNode* node, char* str, int index);

	// ������Ⱥ�������B+�����ͷ����пռ�
	void traverseFreeTree(BNode* root);

	void outputNode(BNode* node);

};

