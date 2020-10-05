#include <math.h>
#include <string.h>
#include <algorithm>
#include "RadixTree.h"

RadixTree::RadixTree(int m)
{
	this->bitsPerLevel = m;
	this->childNum = pow(2, m);
	this->root = new RTNode;
	this->initNode(this->root);
}

RadixTree::~RadixTree()
{
	this->deleteTree(this->root);
}

bool RadixTree::addString(char* str)
{
	RTNode* node = this->root;
	RTNode* parentOfNode = NULL;
	int cellNumber = strlen(str) * 8;
	int position = 0;
	while (position < cellNumber) {
		unsigned char* bitsString = this->getBitsString(str, position, strlen(str));
		if (node->prefix) {
			// ǰ׺�Ƚ�
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, node->bitNumber);
			if (commonBitsNum == node->bitNumber) {
				// ����ƥ��ǰ׺���ƶ�position
				position += node->bitNumber;
				if (position >= cellNumber) {
					node->is_leaf = true;
					break;
				}
			}
			else {
				// ǰ׺����ȫƥ�䣬��Ҫ����
				node = this->devideNode(node, parentOfNode, bitsString, commonBitsNum);
				// ���Ѻ��½����Ľڵ�Ὣstrʣ������bit�洢����
				break;
			}
		}

		// �����Ƚϣ�ȡ�̶�������bit��̽Ѱ�Ƿ��к�̽ڵ�
		unsigned int index = this->getIndex(bitsString);
		if (node->child[index]) {
			parentOfNode = node;
			node = node->child[index];
			position += this->bitsPerLevel;
		}
		else {
			// �½��ڵ�,�洢str��ʣ������bit��Ϊ�ڵ��prefixֵ
			this->createLeafNode(parentOfNode, bitsString, cellNumber - position);
			break;
		}
	}
	return true;
}

bool RadixTree::hasString(char* str)
{
	RTNode* node = this->root;
	int cellNumber = strlen(str) * 8;
	int position = 0;
	while (position < cellNumber) {
		unsigned char* bitsString = this->getBitsString(str, position, strlen(str));
		if (node->prefix) {
			// ǰ׺�Ƚ�
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, node->bitNumber);
			if (commonBitsNum == node->bitNumber) {
				// ����ƥ��ǰ׺���ƶ�position
				position += node->bitNumber;
				if (position >= cellNumber) {
					break;
				}
			}
			else {
				// ǰ׺����ȫƥ�䣬����ʧ��
				return false;
			}
		}

		// �����Ƚϣ�ȡ�̶�������bit��̽Ѱ�Ƿ��к�̽ڵ�
		unsigned int index = this->getIndex(bitsString);
		if (node->child[index]) {
			node = node->child[index];
			position += this->bitsPerLevel;
			continue;
		}
		else {
			return false;
		}
	}
	if (node->is_leaf == true) {
		return true;
	}
	return false;
}

void RadixTree::initNode(RTNode * node)
{
	node->is_leaf = false;
	node->prefix = NULL;
	node->bitNumber = 0;
	node->child = new RTNode*[this->childNum];
	for (int i = 0; i < this->childNum; i++) {
		node->child[i] = NULL;
	}
}

void RadixTree::deleteTree(RTNode* node)
{
	for (int i = 0; i < this->childNum; i++) {
		if (node->child[i]) {
			this->deleteTree(node->child[i]);
		}
	}
	delete node->child;
	delete node;
}

unsigned int RadixTree::getIndex(unsigned char* str)
{	
	// �õ���߼�λ
	unsigned char ch = str[0];
	ch = ch >> 8 - this->bitsPerLevel;

	return (unsigned int)ch;
}

unsigned int RadixTree::getPrefix(unsigned char* str1, unsigned char* str2, unsigned int validBitNum)
{
	unsigned int prefixLength = 0;
	unsigned int index = 0;
	unsigned int maxLength = fmin(strlen((char*)str1), strlen((char*)str2));
	// ����charΪ��λ���бȽ�
	while (index < maxLength) {
		if (str1[index] != str2[index]) {
			break;
		}
		index++;
	}
	if (index == maxLength) {
		return index * 8;
	}

	// char��ƥ�䣬��bitΪ��λ���бȽ�
	unsigned char ch1 = str1[index];
	unsigned char ch2 = str2[index];
	unsigned int bitNum = 0;
	while (bitNum < 8) {
		unsigned char bit1 = ch1 << bitNum;
		bit1 = bit1 >> 7;
		unsigned char bit2 = ch2 << bitNum;
		bit2 = bit2 >> 7;
		if (bit1 != bit2) {
			break;
		}
		bitNum++;
	}
	return index * 8 + bitNum;
}

RTNode* RadixTree::devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum)
{
	// ��node��parent֮������µĸ��ڵ㣬�洢����ǰ׺
	RTNode* commonPrefixNode = new RTNode;
	this->initNode(commonPrefixNode);
	commonPrefixNode->bitNumber = commonBitsNum;
	commonPrefixNode->prefix = this->getBitsString((char*)str, 0, commonBitsNum);

	// �޸�node��ֵ

	// �����½ڵ㣬�洢strʣ������bit
}

RTNode* RadixTree::createLeafNode(RTNode* parent, unsigned char* str, unsigned int lastBitsNumber)
{
	return nullptr;
}

unsigned char * RadixTree::getBitsString(char* str, int start, int end)
{
	return nullptr;
}
