#include <iostream>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <unistd.h>

using namespace std;

// ʹ��unsigned char����ʾ�ֽ�
typedef struct RadixTreeNode {
	bool is_leaf;
	unsigned char* prefix;
	unsigned int bitNumber;
	struct RadixTreeNode** child;
}RTNode;

class RadixTree {
public:
	RadixTree(int);

	~RadixTree();

	bool addString(char*);

	bool hasString(char*);

private:

	RTNode* root = NULL;
	
	int bitsPerLevel;

	int childNum;

	void initNode(RTNode* node);

	void deleteTree(RTNode* node);

	// �ҵ��ַ������ض�λ�õļ���bitֵ����Ϊ�����ӽڵ�����
	unsigned int getIndex(char* str, int position);

	// ���ض��߹�ͬǰ׺��bit��Ŀ
	unsigned int getPrefix(unsigned char* str1, unsigned char* str2, unsigned int maxBitNum);

	// �ڵ����
	RTNode* devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum, unsigned int strBitsNumber);

	// �����µ�Ҷ�ӽڵ㣬�洢[start,end)�ַ���(ǰ��λ�ᱻ֦�洢���Ǵ��ڽڵ���)
	RTNode* createLeafNode(RTNode* parent, char* str, int bitStart, int bitEnd);

	// ����һ���ַ������洢str��[start, end)��Χ�ڵ�bit
	// �±��0��ʼ
	unsigned char* getBitsString(char* str, int start, int end);

	// ȡ���ַ������ض�bit��position��0��ʼ��
	unsigned char getBit(char* str, int position);

};

int main() {

	clock_t start, finish;
	start = clock();

	// ��ÿ���ʾ���ٱ���
	RadixTree RTree(1);
	// ��dict.txt�е��ַ��������ϣ��
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("/home/wanglei/course/dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			RTree.addString(str);
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
			if (RTree.hasString(str)) {
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
			// ǰ׺�Ƚ�
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, fmin(validBitNumber, node->bitNumber));
			if (commonBitsNum == node->bitNumber) {
				// ����ƥ��ǰ׺���ƶ�position
				position += node->bitNumber;
				if (position == cellNumber) {
					node->is_leaf = true;
					break;
				}
			}
			else {
				// ǰ׺����ȫƥ�䣬��Ҫ����
				node = this->devideNode(node, parentOfNode, bitsString, commonBitsNum, validBitNumber);
				// ���Ѻ��½����Ľڵ�Ὣstrʣ������bit�洢����
				break;
			}
		}

		// �����Ƚϣ�ȡ�̶�������bit��̽Ѱ�Ƿ��к�̽ڵ�
		unsigned int index = this->getIndex(str, position);
		if (node->child[index]) {
			parentOfNode = node;
			node = node->child[index];
			position += this->bitsPerLevel;
		}
		else {
			// �½��ڵ�,�洢str��ʣ������bit��Ϊ�ڵ��prefixֵ
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
			// ǰ׺�Ƚ�
			unsigned int commonBitsNum = this->getPrefix(node->prefix, bitsString, fmin(validBitNumber, node->bitNumber));
			if (commonBitsNum == node->bitNumber) {
				// ����ƥ��ǰ׺���ƶ�position
				position += node->bitNumber;
				if (position == cellNumber) {
					return node->is_leaf;
				}
			}
			else {
				// ǰ׺����ȫƥ�䣬����ʧ��
				return false;
			}
		}

		// �����Ƚϣ�ȡ�̶�������bit��̽Ѱ�Ƿ��к�̽ڵ�
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
	// �ҵ�position��Ӧ�ֽ�
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
	// ����charΪ��λ���бȽ�
	while (index < maxLength) {
		if (str1[index] != str2[index]) {
			break;
		}
		index++;
	}
	if (index == maxLength) {
		return fmin(index * 8, maxBitNum);
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
	unsigned int commonBitsNum = index * 8 + bitNum;
	return fmin(commonBitsNum, maxBitNum);
}

RTNode* RadixTree::devideNode(RTNode* node, RTNode* parent, unsigned char* str, unsigned int commonBitsNum, unsigned int strBitsNumber)
{
	// ��commonBitsNum����ΪbitsPerLevel��������
	commonBitsNum = commonBitsNum - commonBitsNum % this->bitsPerLevel;
	// ��node��parent֮������µĸ��ڵ�
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

	// ���¸��ڵ��д洢����ǰ׺
	commonPrefixNode->bitNumber = commonBitsNum;
	if (commonBitsNum > 0) {
		int chNumber = commonBitsNum / 8 + 1;
		commonPrefixNode->prefix = new unsigned char[chNumber + 1];
		strncpy((char*)commonPrefixNode->prefix, (char*)str, chNumber);
		commonPrefixNode->prefix[chNumber] = '\0';
	}

	// �޸�node��ֵ(ԭ��bit�� - �½ڵ��е�bit�� - �¼�һ���ʾ��bit��)
	node->prefix = this->getBitsString((char*)node->prefix, commonBitsNum + this->bitsPerLevel, node->bitNumber);
	node->bitNumber = node->bitNumber - commonBitsNum - this->bitsPerLevel;
	if (node->bitNumber == 0) {
		delete node->prefix;
		node->prefix = NULL;
	}

	// �����½ڵ㣬�洢strʣ������bit
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
	// ��λstart��end�����ֽ�
	int startIndex = start / 8;
	int endIndex = (end - 1) / 8;
	int length = endIndex - startIndex + 1;

	// �½�bitsString�����ض��ַ�����
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

