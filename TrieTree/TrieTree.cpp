#include <iostream>
#include <time.h>
#include <fstream>
#include <string.h>
#include <unistd.h>

using namespace std;

#include <stddef.h>
typedef struct TrieNode {
	// 叶节点标志，num为1表示此节点为一个字符串的结束
	bool is_leaf;
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

int main() {
	clock_t start, finish;
	start = clock();

	TrieTree TrTree;
	// 将dict.txt中的字符串存入哈希表
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("/home/wanglei/course/dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			TrTree.addString(str);
		}
	}
	else {
		cout << "无法打开文件dict.txt" << endl;
	}
	readStr.close();

	// 读取string.txt，逐个检测是否在哈希表中，若在则写入文件reslut.txt
	readStr.open("/home/wanglei/course/string.txt");
	ofstream writeResult;
	int stringMatch = 0;
	writeResult.open("/home/wanglei/course/result/bupt_13.txt");
	if (readStr && writeResult) {
		while (readStr.getline(str, TEMP_SIZE)) {
			if (TrTree.hasString(str)) {
				writeResult << str << '\n';
				stringMatch++;
			}
		}
	}
	else {
		cout << "无法打开输出文件" << endl;
	}
	readStr.close();
	writeResult.close();

	sleep(3);

	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "{runtime:" << duration << "s  " << "string match:" << stringMatch << '}'  << endl;

	return 0;
}
TrieTree::TrieTree() {
	root = new TNode;
	this->FlushNode(root);
}

TrieTree::~TrieTree() {
	// 深度优先遍历TrieTree，删除所有节点
	this->DeleteTree(this->root);
}

bool TrieTree::addString(char* str) {
	TNode* node = this->root;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		int num = (int)str[i];
		num += 128;
		if (node->child[num] == NULL) {
			// 创建新节点
			node->child[num] = new TNode;
			this->FlushNode(node->child[num]);
		}
		node = node->child[num];
	}
	// 标记字符串结束
	node->is_leaf = true;

	return true;
}

bool TrieTree::hasString(char* str)
{
	TNode* node = this->root;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		int num = (int)str[i];
		num += 128;
		if (node->child[num]) {
			node = node->child[num];
		}
		else {
			return false;
		}
	}
	if (node->is_leaf) {
		return true;
	}
	return false;
}

void TrieTree::FlushNode(TNode* node){
	for (int i = 0; i < 256; i++) {
		node->child[i] = NULL;
	}
	// 默认非叶节点
	node->is_leaf = false;
}

void TrieTree::DeleteTree(TNode* node){
	if (node == NULL) {
		return;
	}
	for (int i = 0; i < 256; i++) {
		this->DeleteTree(node->child[i]);
	}
	delete node;
}

