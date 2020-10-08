#include <iostream>
#include <time.h>
#include <fstream>
#include <string.h>
#include <unistd.h>

using namespace std;

#include <stddef.h>
typedef struct TrieNode {
	// Ҷ�ڵ��־��numΪ1��ʾ�˽ڵ�Ϊһ���ַ����Ľ���
	bool is_leaf;
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

int main() {
	clock_t start, finish;
	start = clock();

	TrieTree TrTree;
	// ��dict.txt�е��ַ��������ϣ��
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
		cout << "�޷����ļ�dict.txt" << endl;
	}
	readStr.close();

	// ��ȡstring.txt���������Ƿ��ڹ�ϣ���У�������д���ļ�reslut.txt
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
		cout << "�޷�������ļ�" << endl;
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
	// ������ȱ���TrieTree��ɾ�����нڵ�
	this->DeleteTree(this->root);
}

bool TrieTree::addString(char* str) {
	TNode* node = this->root;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		int num = (int)str[i];
		num += 128;
		if (node->child[num] == NULL) {
			// �����½ڵ�
			node->child[num] = new TNode;
			this->FlushNode(node->child[num]);
		}
		node = node->child[num];
	}
	// ����ַ�������
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
	// Ĭ�Ϸ�Ҷ�ڵ�
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

