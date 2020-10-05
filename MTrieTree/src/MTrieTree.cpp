#include <string.h>
#include <math.h>
#include "MTrieTree.h"

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
