typedef struct BPlusNode {
	int keyNum;
	// 使用字符串前缀4个char作为索引（如果是叶子节点，则存储数据）
	char** Keys;
	// 子节点
	struct BPlusNode** child;
	// 父节点
	struct BPlusNode* parent;
	// 如果是叶子节点，则有next
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

	// B+树的阶数，每个节点最大子节点个数
	int order;

	BNode* searchNode(char* str, bool need_insert);

	// 对输入节点进行节点分裂操作，返回值为新节点
	// < order / 2，左边
	// >= order / 2，右边
	// 如果根节点分裂，自动创建新的根节点
	BNode* devideNode(BNode* node);

	// 创建新节点
	BNode* createNewNode(BNode* parent, bool is_leaf);

	// 在节点中第index个位置插入字符串
	bool insertIntoNode(BNode* node, char* str, int index);

	// 深度优先后续遍历B+树，释放所有空间
	void traverseFreeTree(BNode* root);

	void outputNode(BNode* node);

};

