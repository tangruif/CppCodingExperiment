#include <math.h>
#include <bitset>
#include <queue>
#include <string.h>
#include <algorithm>
#include <iostream>
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
		if (node->prefix) {
			unsigned char* bitsString = this->getBitsString(str, position, strlen(str) * 8);
			unsigned int validBitNumber = strlen(str) * 8 - position;
			// 前缀比较
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, fmin(validBitNumber, node->bitNumber));
			if (commonBitsNum == node->bitNumber) {
				// 完美匹配前缀，移动position
				position += node->bitNumber;
				if (position == cellNumber) {
					node->is_leaf = true;
					break;
				}
			}
			else {
				// 前缀不完全匹配，需要分裂
				node = this->devideNode(node, parentOfNode, bitsString, commonBitsNum, validBitNumber);
				// 分裂后，新建立的节点会将str剩下所有bit存储下来
				break;
			}
		}

		// 正常比较，取固定数量的bit，探寻是否有后继节点
		unsigned int index = this->getIndex(str, position);
		if (node->child[index]) {
			parentOfNode = node;
			node = node->child[index];
			position += this->bitsPerLevel;
		}
		else {
			// 新建节点,存储str中剩下所有bit作为节点的prefix值
			this->createLeafNode(node, str, position, strlen(str) * 8);
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
		if (node->prefix) {
			unsigned char* bitsString = this->getBitsString(str, position, strlen(str) * 8);
			unsigned int validBitNumber = strlen(str) * 8 - position;
			// 前缀比较
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, fmin(validBitNumber, node->bitNumber));
			if (commonBitsNum == node->bitNumber) {
				// 完美匹配前缀，移动position
				position += node->bitNumber;
				if (position == cellNumber) {
					return node->is_leaf;
				}
			}
			else {
				// 前缀不完全匹配，查找失败
				return false;
			}
		}

		// 正常比较，取固定数量的bit，探寻是否有后继节点
		unsigned int index = this->getIndex(str, position);
		if (node->child[index]) {
			node = node->child[index];
			position += this->bitsPerLevel;
			continue;
		}
		else {
			return false;
		}
	}
	if (node->is_leaf == true && node->bitNumber == 0) {
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

unsigned int RadixTree::getIndex(char* str, int position)
{	
	// 找到position对应字节
	int chIndex = position / 8;
	int bitOffset = position % 8;
	unsigned char ch = str[chIndex];
	ch = ch << bitOffset;
	ch = ch >> (8 - this->bitsPerLevel);

	return (unsigned int)ch;
}

unsigned int RadixTree::getPrefix(unsigned char* str1, unsigned char* str2, unsigned int maxBitNum)
{
	unsigned int index = 0;
	unsigned int maxLength = (maxBitNum - 1) / 8 + 1;
	// 先以char为单位进行比较
	while (index < maxLength) {
		if (str1[index] != str2[index]) {
			break;
		}
		index++;
	}
	if (index == maxLength) {
		return fmin(index * 8, maxBitNum);
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
	unsigned int commonBitsNum = index * 8 + bitNum;
	return fmin(commonBitsNum, maxBitNum);
}

RTNode* RadixTree::devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum, unsigned int strBitsNumber)
{
	// 将commonBitsNum调整为bitsPerLevel的整数倍
	commonBitsNum = commonBitsNum - commonBitsNum % this->bitsPerLevel;
	// 在node和parent之间插入新的父节点
	RTNode* commonPrefixNode = new RTNode;
	this->initNode(commonPrefixNode);
	if (parent) {
		for (int i = 0; i < this->childNum; i++) {
			if (parent->child[i] == node) {
				parent->child[i] = commonPrefixNode;
				break;
			}
		}
	}
	commonPrefixNode->child[this->getIndex((char*)node->prefix, commonBitsNum)] = node;

	// 在新父节点中存储公共前缀
	commonPrefixNode->bitNumber = commonBitsNum;
	if (commonBitsNum > 0) {
		int chNumber = commonBitsNum / 8 + 1;
		commonPrefixNode->prefix = new unsigned char[chNumber + 1];
		strncpy((char*)commonPrefixNode->prefix, (char*)str, chNumber);
		commonPrefixNode->prefix[chNumber] = '\0';
	}

	// 修改node的值(原有bit数 - 新节点中的bit数 - 新加一层表示的bit数)
	node->prefix = this->getBitsString((char*)node->prefix, commonBitsNum + this->bitsPerLevel, node->bitNumber);
	node->bitNumber = node->bitNumber - commonBitsNum - this->bitsPerLevel;
	if (node->bitNumber == 0) {
		delete node->prefix;
		node->prefix = NULL;
	}

	// 创建新节点，存储str剩下所有bit
	RTNode* newNode = this->createLeafNode(commonPrefixNode, (char*)str, commonBitsNum, strBitsNumber);

	return newNode;
}

RTNode* RadixTree::createLeafNode(RTNode* parent, char* str, int bitStart, int bitEnd)
{
	RTNode* newLeafNode = new RTNode;
	this->initNode(newLeafNode);
	if (parent) {
		parent->child[this->getIndex(str, bitStart)] = newLeafNode;
	}
	int prefixBitStart = bitStart + this->bitsPerLevel;
	newLeafNode->bitNumber = bitEnd - prefixBitStart;
	newLeafNode->prefix = this->getBitsString(str, prefixBitStart, bitEnd);
	newLeafNode->is_leaf = true;

	return newLeafNode;
}

unsigned char* RadixTree::getBitsString(char* str, int start, int end)
{
	// 定位start与end所在字节
	int startIndex = start / 8;
	int endIndex = (end - 1) / 8;
	int length = endIndex - startIndex + 1;

	// 新建bitsString，将特定字符放入
	unsigned char* bitsString = new unsigned char[length + 1];
	int position = start;
	for (int i = 0; i < length; i++) {
		bitsString[i] = 0;
		for (int j = 0; j < 8; j++) {
			if (position >= end) {
				bitsString[i] = bitsString[i] << 1;
			}
			else {
				bitsString[i] = (bitsString[i] << 1) + getBit(str, position);
				position++;
			}
		}
	}
	bitsString[length] = '\0';

	return bitsString;
}

unsigned char RadixTree::getBit(char * str, int position)
{
	unsigned char bit = str[position / 8] << (position % 8);
	bit = bit >> 7;
	return bit;
}

void RadixTree::outputAllNodes()
{
	std::queue<RTNode*> nodeQueue;
	nodeQueue.push(this->root);
	while (!nodeQueue.empty()) {
		std::queue<RTNode*> nextLevelQueue;
		while (!nodeQueue.empty()) {
			RTNode* node = nodeQueue.front();
			nodeQueue.pop();
			std::cout << '{';
			if (node->prefix) {
				for (unsigned int i = 0; i < strlen((char*)node->prefix); i++) {
					std::cout << std::bitset<8>(node->prefix[i]) << ' ';
				}
			}
			else {
				std::cout << "NULL";
			}
			std::cout << " bits:" << node->bitNumber;
			std::cout << '}';
			for (int i = 0; i < this->childNum; i++) {
				if (node->child[i]) {
					nextLevelQueue.push(node->child[i]);
				}
			}
		}
		nodeQueue = nextLevelQueue;
		std::cout << std::endl;
	}
	std::cout << "over\n" << std::endl;
}
