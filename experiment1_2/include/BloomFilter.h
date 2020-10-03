class BloomFilter {
public:

	/* ������
		1. ���洢���ַ�������
		2. �ڴ��Ĵ�����
	*/
	BloomFilter(long long, double);

	~BloomFilter();


	bool addString(char* str);

	bool hasString(char* str);

private:

	// �ڴ��Ĵ�����
	double WrongRate;

	// �����ϣ�����ĸ���
	long hashFuncNum;

	// bit�����С 
	long long filterSize;

	// bit ����
	char* Filter;

	// ʹ��murmurHash�����������㹻���hash����
	unsigned int murmurHash(char*, const int);
};