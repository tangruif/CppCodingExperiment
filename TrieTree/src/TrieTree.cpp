#include <string.h>
#include "TrieTree.h"

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

