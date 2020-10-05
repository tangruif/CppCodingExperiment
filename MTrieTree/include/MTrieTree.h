#include <stdlib.h>
typedef struct MTrieNode {
	bool is_leaf;
	struct MTrieNode** child;
}MTNode;

class MTrieTree {
public:

	// ȷ����������Ϊ���ٲ�(��ȡ2,4,16,256��)
	MTrieTree(int m);

	~MTrieTree();

	bool addString(char* str);

	bool hasString(char* str);

private:

	MTNode* root = NULL;

	// ���ж��ٲ�
	int m;

	// ÿ��ڵ����ı�����
	int bitsPerLevel;

	void initNode(MTNode* node);

	void deleteTree(MTNode* node);

	unsigned int getIndex(char ch, int i);

};