#include <stddef.h>
typedef struct TrieNode {
	// 叶节点标志，num为1表示此节点为一个字符串的结束
	bool num;
	// 一个字节，范围:-128~127
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