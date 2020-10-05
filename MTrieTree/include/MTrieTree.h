#include <stdlib.h>
typedef struct MTrieNode {
	bool is_leaf;
	struct MTrieNode** child;
}MTNode;

class MTrieTree {
public:

	// 确定建立的树为多少叉(可取2,4,16,256叉)
	MTrieTree(int m);

	~MTrieTree();

	bool addString(char* str);

	bool hasString(char* str);

private:

	MTNode* root = NULL;

	// 树有多少叉
	int m;

	// 每层节点代表的比特数
	int bitsPerLevel;

	void initNode(MTNode* node);

	void deleteTree(MTNode* node);

	unsigned int getIndex(char ch, int i);

};