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
			// 前缀比较
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, node->bitNumber);
			if (commonBitsNum == node->bitNumber) {
				// 完美匹配前缀，移动position
				position += node->bitNumber;
				if (position >= cellNumber) {
					node->is_leaf = true;
					break;
				}
			}
			else {
				// 前缀不完全匹配，需要分裂
				node = this->devideNode(node, parentOfNode, bitsString, commonBitsNum);
				// 分裂后，新建立的节点会将str剩下所有bit存储下来
				break;
			}
		}

		// 正常比较，取固定数量的bit，探寻是否有后继节点
		unsigned int index = this->getIndex(bitsString);
		if (node->child[index]) {
			parentOfNode = node;
			node = node->child[index];
			position += this->bitsPerLevel;
		}
		else {
			// 新建节点,存储str中剩下所有bit作为节点的prefix值
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
			// 前缀比较
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, node->bitNumber);
			if (commonBitsNum == node->bitNumber) {
				// 完美匹配前缀，移动position
				position += node->bitNumber;
				if (position >= cellNumber) {
					break;
				}
			}
			else {
				// 前缀不完全匹配，查找失败
				return false;
			}
		}

		// 正常比较，取固定数量的bit，探寻是否有后继节点
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
	// 得到最高几位
	unsigned char ch = str[0];
	ch = ch >> 8 - this->bitsPerLevel;

	return (unsigned int)ch;
}

unsigned int RadixTree::getPrefix(unsigned char* str1, unsigned char* str2, unsigned int validBitNum)
{
	unsigned int prefixLength = 0;
	unsigned int index = 0;
	unsigned int maxLength = fmin(strlen((char*)str1), strlen((char*)str2));
	// 先以char为单位进行比较
	while (index < maxLength) {
		if (str1[index] != str2[index]) {
			break;
		}
		index++;
	}
	if (index == maxLength) {
		return index * 8;
	}

	// char不匹配，以bit为单位进行比较
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
	// 在node和parent之间插入新的父节点，存储公共前缀
	RTNode* commonPrefixNode = new RTNode;
	this->initNode(commonPrefixNode);
	commonPrefixNode->bitNumber = commonBitsNum;
	commonPrefixNode->prefix = this->getBitsString((char*)str, 0, commonBitsNum);

	// 修改node的值

	// 创建新节点，存储str剩下所有bit
}

RTNode* RadixTree::createLeafNode(RTNode* parent, unsigned char* str, unsigned int lastBitsNumber)
{
	return nullptr;
}

unsigned char * RadixTree::getBitsString(char* str, int start, int end)
{
	return nullptr;
}
