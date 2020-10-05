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

	void outputAllNodes();

private:

	RTNode* root = NULL;
	
	int bitsPerLevel;

	int childNum;

	void initNode(RTNode* node);

	void deleteTree(RTNode* node);

	// �ҵ��ַ������ض�λ�õļ���bitֵ����Ϊ�����ӽڵ�����
	unsigned int getIndex(char* str, int position);

	// ���ض��߹�ͬǰ׺��bit��Ŀ
	unsigned int getPrefix(unsigned char* str1, unsigned char* str2, unsigned int maxBitNum);

	// �ڵ����
	RTNode* devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum, unsigned int strBitsNumber);

	// �����µ�Ҷ�ӽڵ㣬�洢[start,end)�ַ���(ǰ��λ�ᱻ֦�洢���Ǵ��ڽڵ���)
	RTNode* createLeafNode(RTNode* parent, char* str, int bitStart, int bitEnd);

	// ����һ���ַ������洢str��[start, end)��Χ�ڵ�bit
	// �±��0��ʼ
	unsigned char* getBitsString(char* str, int start, int end);

	// ȡ���ַ������ض�bit��position��0��ʼ��
	unsigned char getBit(char* str, int position);

};