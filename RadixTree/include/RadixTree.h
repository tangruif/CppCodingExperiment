// 使用unsigned char来表示字节
typedef struct RadixTreeNode {
	bool is_leaf;
	unsigned char* prefix;
	unsigned int bitNumber;
	struct RadixTreeNode** child;
}RTNode;

class RadixTree {
public:
	RadixTree(int);

	~RadixTree();

	bool addString(char*);

	bool hasString(char*);

	void outputAllNodes();

private:

	RTNode* root = NULL;
	
	int bitsPerLevel;

	int childNum;

	void initNode(RTNode* node);

	void deleteTree(RTNode* node);

	// 找到字符串中特定位置的几个bit值，作为树的子节点索引
	unsigned int getIndex(char* str, int position);

	// 返回二者共同前缀的bit数目
	unsigned int getPrefix(unsigned char* str1, unsigned char* str2, unsigned int maxBitNum);

	// 节点分裂
	RTNode* devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum, unsigned int strBitsNumber);

	// 创建新的叶子节点，存储[start,end)字符串(前几位会被枝存储而非存在节点内)
	RTNode* createLeafNode(RTNode* parent, char* str, int bitStart, int bitEnd);

	// 返回一个字符串，存储str中[start, end)范围内的bit
	// 下标从0开始
	unsigned char* getBitsString(char* str, int start, int end);

	// 取出字符串中特定bit（position从0开始）
	unsigned char getBit(char* str, int position);

};