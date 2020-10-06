#include <string.h>
#include<iostream>
#include <queue>
#include "BPlusTree.h"


BPlusTree::BPlusTree(int order):
	order(order)
{
	root = this->createNewNode(NULL, true);
}

BPlusTree::~BPlusTree() {
	// 遍历B+树，空间全部释放
	this->traverseFreeTree(this->root);
}

bool BPlusTree::addString(char* str) {
	// 找到待插入的叶子节点
	BNode* leafNode = this->searchNode(str, true);

	// 找到需插入的位置
	char** allStr = leafNode->Keys;
	int insert_index = 0;
	for (insert_index = 0; insert_index < leafNode->keyNum; insert_index++) {
		if (strcmp(str, allStr[insert_index]) < 0) {
			break;
		}
	}
	// 将str插入leafNode
	this->insertIntoNode(leafNode, str, insert_index);

	if (leafNode->keyNum > order) { 
		// 节点分裂
		this->devideNode(leafNode);

		// 向上继续分裂
		BNode* fatherNode = leafNode->parent;
		while (fatherNode->keyNum > this->order) {
			this->devideNode(fatherNode);
			fatherNode = fatherNode->parent;
		}
	}

	return true;
}

bool BPlusTree::hasString(char * str)
{
	// 搜索找到字符串所在的叶子节点
	BNode* node = this->searchNode(str, false);
	if (node == NULL) {
		return false;
	}


	// 遍历叶子节点当中的所有元素，匹配是否有相同的字符串
	for (int i = 0; i < node->keyNum; i++) {
		int cmp_result = strcmp(str, node->Keys[i]);
		if (cmp_result == 0) {
			return true;
		}
		else if (cmp_result < 0) {
			return false;
		}
	}
	return false;
}

void BPlusTree::outputTree()
{
	// 逐层输出B+树的key值
	// 分层进行广度优先遍历
	std::queue<BNode*> traverseQueue;
	traverseQueue.push(this->root);
	while (!traverseQueue.empty()) {
		std::queue<BNode*> nextLevelQueue;
		// 将一层的所有节点全遍历完
		while (!traverseQueue.empty()) {
			BNode* node = traverseQueue.front();
			traverseQueue.pop();
			this->outputNode(node);
			if (node->child) {
				// 将node的子节点加入队列
				for (int i = 0; i < node->keyNum; i++) {
					nextLevelQueue.push(node->child[i]);
				}
			}
		}
		traverseQueue = nextLevelQueue;
		std::cout << std::endl;
	}
}

BNode* BPlusTree::searchNode(char* str, bool need_insert) {
	BNode* node = root;

	// 若找到叶子节点，则返回
	while (node->child != NULL) {
		bool is_find = false;
		for (int i = 0; i < node->keyNum; i++) {
			if (strcmp(str, node->Keys[i]) <= 0) {
				node = node->child[i];
				is_find = true;
				break;
			}
		}
		// 未找到，则说明比子树下所有节点都大
		if (!is_find) {
			if (need_insert == false) {
				// 若只查找，则查找失败
				return NULL;
			}
			else {
				// 若还需插入，则选择最后一个节点进行插入，并更新最大值
				delete[] node->Keys[node->keyNum - 1];
				node->Keys[node->keyNum - 1] = new char[strlen(str) + 1];
				strcpy(node->Keys[node->keyNum - 1], str);
				node = node->child[node->keyNum - 1];
			}
		}
	}

	return node;
}

BNode* BPlusTree::devideNode(BNode* node) {
	// 创建新节点
	bool is_leaf = (node->child == NULL);
	BNode* newNode = this->createNewNode(node->parent, is_leaf);
	newNode->next = node->next;
	if (is_leaf) {
		// 叶子节点需要互相连接
		node->next = newNode;
	}

	// 将旧节点中一半数据（和child一起）和转移到新节点
	int partition = order / 2;
	for (int i = partition + 1; i <= order; i ++) {
		newNode->Keys[i - partition - 1] = node->Keys[i];
		node->Keys[i] = NULL;
		if (!is_leaf) {
			node->child[i]->parent = newNode;
			newNode->child[i - partition - 1] = node->child[i];
			node->child[i] = NULL;
		}
	}
	node->keyNum = partition + 1;
	newNode->keyNum = order - partition;

	// 如果存在父节点，则为其增加子节点，否则新增父节点
	BNode* parent = node->parent;
	if (parent) {
		parent->keyNum ++;
		// 将newNode插入到node后一个
		for (int i = parent->keyNum - 1; i > 0; i--) {
			if (parent->child[i - 1] != node) {
				parent->child[i] = parent->child[i - 1];
				parent->Keys[i] = parent->Keys[i - 1];
			}
			else {
				parent->child[i] = newNode;

				// 存储子节点中最大值
				parent->Keys[i] = new char[strlen(newNode->Keys[newNode->keyNum - 1]) + 1]; 
				strcpy(parent->Keys[i], newNode->Keys[newNode->keyNum - 1]);

				// 更新被分裂节点的父节点的key值
				delete[] parent->Keys[i - 1];
				parent->Keys[i - 1] = new char[strlen(node->Keys[node->keyNum - 1]) + 1];
				strcpy(parent->Keys[i - 1], node->Keys[node->keyNum - 1]);

				break;
			}
		}
	}
	else {
		// 新增节点为根节点
		BNode* newRoot = this->createNewNode(NULL, false);
		node->parent = newRoot;
		newNode->parent = newRoot;
		this->root = newRoot;
		parent = newRoot;

		// 将node与newNode信息计入根节点
		newRoot->keyNum = 2;
		newRoot->child[0] = node;
		newRoot->child[1] = newNode;

		newRoot->Keys[0] = new char[strlen(node->Keys[node->keyNum - 1]) + 1];
		strcpy(newRoot->Keys[0], node->Keys[node->keyNum - 1]);
		newRoot->Keys[1] = new char[strlen(newNode->Keys[newNode->keyNum - 1]) + 1];
		strcpy(newRoot->Keys[1], newNode->Keys[newNode->keyNum - 1]);
	}

	return newNode;
}

BNode* BPlusTree::createNewNode(BNode* parent, bool is_leaf) {
	BNode* newNode = new BNode;
	newNode->keyNum = 0;
	// 多分配一个位置，用作节点分裂前的缓冲
	newNode->Keys = new char*[this->order + 1];
	if (!is_leaf) {
		newNode->child = new BNode*[this->order + 1];
	}
	else {
		newNode->child = NULL;
	}
	for (int i = 0; i <= this->order; i++) {
		newNode->Keys[i] = NULL;
		if (!is_leaf) {
			newNode->child[i] = NULL;
		}
	}
	newNode->parent = parent;
	newNode->next = NULL;

	return newNode;
}

bool BPlusTree::insertIntoNode(BNode* node, char* str, int index) {
	if (index < 0 || index > order || node->keyNum  > this->order) {
		return false;
	}
	for (int i = order; i > index; i --) {
		node->Keys[i] = node->Keys[i - 1];
	}
	node->keyNum ++;
	node->Keys[index] = new char[strlen(str) + 1];
	strcpy(node->Keys[index], str);
	return true;
}


void BPlusTree::traverseFreeTree(BNode* root)
{
	if (root->child) {
		for (int i = 0; i < root->keyNum; i++) {
			this->traverseFreeTree(root->child[i]);
		}
		delete[] root->child;
	}
	for (int i = 0; i < root->keyNum; i++) {
		delete[] root->Keys[i];
	}
	delete[] root->Keys;
	delete root;
}

void BPlusTree::outputNode(BNode* node)
{
	std::cout << '{';
	for (int i = 0; i < node->keyNum; i++) {
		std::cout << '\"' << node->Keys[i] << '\"';
	}
	std::cout << '}';
}
