#include "BloomFilter.h"
#include <string.h>
#include <math.h>
#include <algorithm>

BloomFilter::BloomFilter(long long strNum, double expectWrongRate) {
	/* ���ݣ�
	   1. �ַ�������
	   2. �ڴ��Ĵ�����
	   ���㣺
	   1. �����ٵĴ洢�ռ�λ��
	   2. ��ϣ��������
	*/
	// ��������ֵ
	strNum = std::max((long long)1, strNum);
	expectWrongRate = fmin(1, expectWrongRate);
	expectWrongRate = fmax(0, expectWrongRate);

	// ������
	this->WrongRate = expectWrongRate;
	this->filterSize = (long long)(strNum * 1.44 * log2(1 / expectWrongRate));
	this->hashFuncNum = (long)(0.693 * filterSize / strNum);

	// ����bit���飬��ʼ��
	Filter = new char[this->filterSize];
	memset(Filter, 0, this->filterSize);
}

BloomFilter::~BloomFilter() {
	delete[] Filter;
}

bool BloomFilter::addString(char* str) {
	// ʹ��hashFuncNum��hash�������������ַ������д���
	for (int i = 1; i <= this->hashFuncNum; i++) {
		unsigned int hashNum = this->murmurHash(str, i) % this->filterSize;
		this->Filter[hashNum] = 1;
	}
	return true;
}

bool BloomFilter::hasString(char* str) {
	// ʹ��hashFuncNum��hash�������������ַ������д���
	for (int i = 1; i <= this->hashFuncNum; i++) {
		unsigned int hashNum = this->murmurHash(str, i) % this->filterSize;
		if (this->Filter[hashNum] == 0) {
			return false;
		}
	}
	return true;
}

unsigned int BloomFilter::murmurHash(char* str, const int seed) {
	const unsigned int m = 0x5bd1e995;
    const int r = 24;
    int len = strlen(str);
    unsigned int h = seed ^ len;
    // Mix 4 bytes at a time into the hash
    const unsigned char *data = (const unsigned char *)str;
    while(len >= 4)
    {
    	unsigned int k = *(unsigned int *)data;
    	k *= m; 
    	k ^= k >> r; 
    	k *= m; 
    	h *= m; 
    	h ^= k;
    	data += 4;
    	len -= 4;
    }
    // Handle the last few bytes of the input array
    switch(len)
    {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
        h *= m;
    };
    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}
