#include "BloomFilter.h"
#include <string.h>
#include <math.h>
#include <algorithm>

BloomFilter::BloomFilter(long long strNum, double expectWrongRate) {
	/* 根据：
	   1. 字符串个数
	   2. 期待的错误率
	   计算：
	   1. 待开辟的存储空间位数
	   2. 哈希函数个数
	*/
	// 矫正输入值
	strNum = std::max((long long)1, strNum);
	expectWrongRate = fmin(1, expectWrongRate);
	expectWrongRate = fmax(0, expectWrongRate);

	// 计算结果
	this->WrongRate = expectWrongRate;
	this->filterSize = (long long)(strNum * 1.44 * log2(1 / expectWrongRate));
	this->hashFuncNum = (long)(0.693 * filterSize / strNum);

	// 创建bit数组，初始化
	Filter = new char[this->filterSize];
	memset(Filter, 0, this->filterSize);
}

BloomFilter::~BloomFilter() {
	delete[] Filter;
}

bool BloomFilter::addString(char* str) {
	// 使用hashFuncNum个hash函数，对输入字符串进行处理
	for (int i = 1; i <= this->hashFuncNum; i++) {
		unsigned int hashNum = this->murmurHash(str, i) % this->filterSize;
		this->Filter[hashNum] = 1;
	}
	return true;
}

bool BloomFilter::hasString(char* str) {
	// 使用hashFuncNum个hash函数，对输入字符串进行处理
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
