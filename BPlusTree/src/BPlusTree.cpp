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
	// ����B+�����ռ�ȫ���ͷ�
	this->traverseFreeTree(this->root);
}

bool BPlusTree::addString(char* str) {
	// �ҵ��������Ҷ�ӽڵ�
	BNode* leafNode = this->searchNode(str, true);

	// �ҵ�������λ��
	char** allStr = leafNode->Keys;
	int insert_index = 0;
	for (insert_index = 0; insert_index < leafNode->keyNum; insert_index++) {
		if (strcmp(str, allStr[insert_index]) < 0) {
			break;
		}
	}
	// ��str����leafNode
	this->insertIntoNode(leafNode, str, insert_index);

	if (leafNode->keyNum > order) { 
		// �ڵ����
		this->devideNode(leafNode);

		// ���ϼ�������
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
	// �����ҵ��ַ������ڵ�Ҷ�ӽڵ�
	BNode* node = this->searchNode(str, false);
	if (node == NULL) {
		return false;
	}


	// ����Ҷ�ӽڵ㵱�е�����Ԫ�أ�ƥ���Ƿ�����ͬ���ַ���
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
	// ������B+����keyֵ
	// �ֲ���й�����ȱ���
	std::queue<BNode*> traverseQueue;
	traverseQueue.push(this->root);
	while (!traverseQueue.empty()) {
		std::queue<BNode*> nextLevelQueue;
		// ��һ������нڵ�ȫ������
		while (!traverseQueue.empty()) {
			BNode* node = traverseQueue.front();
			traverseQueue.pop();
			this->outputNode(node);
			if (node->child) {
				// ��node���ӽڵ�������
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

	// ���ҵ�Ҷ�ӽڵ㣬�򷵻�
	while (node->child != NULL) {
		bool is_find = false;
		for (int i = 0; i < node->keyNum; i++) {
			if (strcmp(str, node->Keys[i]) <= 0) {
				node = node->child[i];
				is_find = true;
				break;
			}
		}
		// δ�ҵ�����˵�������������нڵ㶼��
		if (!is_find) {
			if (need_insert == false) {
				// ��ֻ���ң������ʧ��
				return NULL;
			}
			else {
				// ��������룬��ѡ�����һ���ڵ���в��룬���������ֵ
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
	// �����½ڵ�
	bool is_leaf = (node->child == NULL);
	BNode* newNode = this->createNewNode(node->parent, is_leaf);
	newNode->next = node->next;
	if (is_leaf) {
		// Ҷ�ӽڵ���Ҫ��������
		node->next = newNode;
	}

	// ���ɽڵ���һ�����ݣ���childһ�𣩺�ת�Ƶ��½ڵ�
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

	// ������ڸ��ڵ㣬��Ϊ�������ӽڵ㣬�����������ڵ�
	BNode* parent = node->parent;
	if (parent) {
		parent->keyNum ++;
		// ��newNode���뵽node��һ��
		for (int i = parent->keyNum - 1; i > 0; i--) {
			if (parent->child[i - 1] != node) {
				parent->child[i] = parent->child[i - 1];
				parent->Keys[i] = parent->Keys[i - 1];
			}
			else {
				parent->child[i] = newNode;

				// �洢�ӽڵ������ֵ
				parent->Keys[i] = new char[strlen(newNode->Keys[newNode->keyNum - 1]) + 1]; 
				strcpy(parent->Keys[i], newNode->Keys[newNode->keyNum - 1]);

				// ���±����ѽڵ�ĸ��ڵ��keyֵ
				delete[] parent->Keys[i - 1];
				parent->Keys[i - 1] = new char[strlen(node->Keys[node->keyNum - 1]) + 1];
				strcpy(parent->Keys[i - 1], node->Keys[node->keyNum - 1]);

				break;
			}
		}
	}
	else {
		// �����ڵ�Ϊ���ڵ�
		BNode* newRoot = this->createNewNode(NULL, false);
		node->parent = newRoot;
		newNode->parent = newRoot;
		this->root = newRoot;
		parent = newRoot;

		// ��node��newNode��Ϣ������ڵ�
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
	// �����һ��λ�ã������ڵ����ǰ�Ļ���
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
