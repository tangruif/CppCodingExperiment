#include <iostream>
#include <time.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

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

int main() {
	clock_t start, finish;
	start = clock();

	// 树分多少叉
	MTrieTree MTrTree(2);
	// 将dict.txt中的字符串存入哈希表
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("/home/wanglei/course/dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			MTrTree.addString(str);
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
			if (MTrTree.hasString(str)) {
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

MTrieTree::MTrieTree(int m):
	m(m)
{
	this->root = new MTNode;
	this->initNode(this->root);
	this->bitsPerLevel = (int)log2(m);
	this->bitsPerLevel = bitsPerLevel > 8 ? 8 : bitsPerLevel;
}

MTrieTree::~MTrieTree()
{
	// 深度优先遍历，释放所有节点
	this->deleteTree(this->root);
}

bool MTrieTree::addString(char * str)
{
	MTNode* node = root;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		// 将每个字符中的bit逐位取出，进行比较
		char ch = str[i];
		int times = 8 / this->bitsPerLevel;
		for (int i = 0; i < times; i++) {
			unsigned int index = this->getIndex(ch, i);
			if (node->child[index] == NULL) {
				node->child[index] = new MTNode;
				this->initNode(node->child[index]);
			}
			node = node->child[index];
		}
	}
	node->is_leaf = true;
	return false;
}

bool MTrieTree::hasString(char * str)
{
	MTNode* node = root;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		// 将每个字符中的bit逐位取出，进行比较
		char ch = str[i];
		int times = 8 / this->bitsPerLevel;
		for (int i = 0; i < times; i++) {
			unsigned int index = this->getIndex(ch, i);
			if (node->child[index] == NULL) {
				return false;
			}
			node = node->child[index];
		}
	}
	if (node->is_leaf == true) {
		return true;
	}
	return false;
}

void MTrieTree::initNode(MTNode* node)
{
	node->is_leaf = false;
	node->child = new MTNode*[this->m];
	for (int i = 0; i < this->m; i++) {
		node->child[i] = NULL;
	} 
}

void MTrieTree::deleteTree(MTNode* node)
{
	if (node == NULL) {
		return;
	}
	for (int i = 0; i < this->m; i++) {
		this->deleteTree(node->child[i]);
	}
	delete[] node->child;
	delete node;
}

unsigned int MTrieTree::getIndex(char ch, int i) {

	int times = 8 / this->bitsPerLevel;
	unsigned char index = ch;
	// 将需要的位数移动至最高位
	index = index << (this->bitsPerLevel * i);
	// 将需要的位数移动至最低位
	index = index >> (8 - this->bitsPerLevel);
	return (unsigned int)index;
}
