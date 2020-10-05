// ʹ��unsigned char����ʾ�ֽ�
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

private:

	RTNode* root = NULL;
	
	int bitsPerLevel;

	int childNum;

	void initNode(RTNode* node);

	void deleteTree(RTNode* node);

	// �ҵ��ַ�����ǰ����bitֵ����Ϊ�����ӽڵ�����
	unsigned int getIndex(unsigned char* str);

	// ���ض��߹�ͬǰ׺��bit��Ŀ
	unsigned int getPrefix(unsigned char* str1, unsigned char* str2, unsigned int validBitNum);

	// �ڵ����
	RTNode* devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum);

	// �����µ�Ҷ�ӽڵ㣬�洢�ַ���
	RTNode* createLeafNode(RTNode* parent, unsigned char* str, unsigned int lastBitsNumber);

	// ����һ���ַ������洢str��[start, end)��Χ�ڵ�bit
	// �±��0��ʼ
	unsigned char* getBitsString(char* str, int start, int end);
};