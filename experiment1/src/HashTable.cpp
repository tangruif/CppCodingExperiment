#include "HashTable.h"
#include <string.h>

HashTable::HashTable() {
	this->hashTable = new HashBucket[this->BUCKET_NUM];
	for (int i = 0; i < this->BUCKET_NUM; ++i) {
		this->hashTable[i].link = NULL;
	}
}

HashTable::~HashTable() {
	// ����hashTable���ͷ������ڴ�ռ�
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

	// �½��ڵ�
	StringLink* newNode = new StringLink;
	newNode->next = NULL;
	newNode->str = new char[strlen(str) + 1];
	strcpy(newNode->str, str);

	// �����ϣ��ͷ��������
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
		hash = (hash << 4) + (*str++);// hash����4λ������ǰ�ַ�ASCII����hash����λ
		if ((x = hash & 0xF0000000L)) {
			hash ^= (x >> 24);
			hash &= ~x;
		}
	}
	return (hash & 0x7FFFFFFF);
}