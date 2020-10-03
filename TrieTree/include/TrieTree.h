#include <stddef.h>
typedef struct TrieNode {
	// Ҷ�ڵ��־��numΪ1��ʾ�˽ڵ�Ϊһ���ַ����Ľ���
	bool num;
	// һ���ֽڣ���Χ:-128~127
	struct TrieNode* child[256];
}TNode;

class TrieTree {
public:
	TrieTree();

	~TrieTree();

	bool addString(char* str);

	bool hasString(char* str);

private:

	TNode* root = NULL;

	void FlushNode(TNode* node);

	void DeleteTree(TNode* node);

};