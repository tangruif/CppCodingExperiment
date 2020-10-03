class BloomFilter {
public:

	/* 参数：
		1. 待存储的字符串个数
		2. 期待的错误率
	*/
	BloomFilter(long long, double);

	~BloomFilter();


	bool addString(char* str);

	bool hasString(char* str);

private:

	// 期待的错误率
	double WrongRate;

	// 所需哈希函数的个数
	long hashFuncNum;

	// bit数组大小 
	long long filterSize;

	// bit 数组
	char* Filter;

	// 使用murmurHash函数来生成足够多的hash函数
	unsigned int murmurHash(char*, const int);
};