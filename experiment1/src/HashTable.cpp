#include "HashTable.h"
#include <string.h>

HashTable::HashTable() {
	this->hashTable = new HashBucket[this->BUCKET_NUM];
	for (int i = 0; i < this->BUCKET_NUM; ++i) {
		this->hashTable[i].link = NULL;
	}
}

HashTable::~HashTable() {
	// 遍历hashTable，释放所有内存空间
	for (int i = 0; i < this->BUCKET_NUM; ++i) {
		StringLink* node = this->hashTable[i].link;
		if (node) {
			StringLink* nextNode = NULL;
			while (node != NULL) {
				nextNode = node->next;
				delete node;
				node = nextNode;
			}
		}
	}
	delete[] this->hashTable;
}

bool HashTable::hasString(char* str) {
	StringLink* node = this->getNode(str);
	if (node) {
		return true;
	}
	else {
		return false;
	}
}

bool HashTable::addString(char* str) {
	HashBucket* bucket = this->getBucket(str);

	if (bucket == NULL) {
		return false;
	}

	// 新建节点
	StringLink* newNode = new StringLink;
	newNode->next = NULL;
	newNode->str = new char[strlen(str) + 1];
	strcpy(newNode->str, str);

	// 加入哈希表，头插入链表
	StringLink* node = bucket->link;
	bucket->link = newNode;
	newNode->next = node;

	return true;
}

StringLink* HashTable::getNode(char* str) {
	HashBucket* bucket = this->getBucket(str);

	if (bucket == NULL) {
		return NULL;
	}

	StringLink* node = bucket->link;
	if (node) {
		while (strcmp(str, node->str) != 0) {
			if (node->next != NULL) {
				node = node->next;
			}
			else {
				return NULL;
			}
		}
	}
	return node;
}

HashBucket* HashTable::getBucket(char* str) {
	unsigned int index = this->ELFHash(str);
	index = index % this->BUCKET_NUM;
	return &this->hashTable[index];
}

unsigned int HashTable::ELFHash(char* str) {
	unsigned int hash = 0;
	unsigned int x = 0;
	while (*str) {
		hash = (hash << 4) + (*str++);// hash左移4位，将当前字符ASCII存入hash低四位
		if ((x = hash & 0xF0000000L)) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return (hash & 0x7FFFFFFF);
}