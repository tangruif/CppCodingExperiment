#include <iostream>
#include <time.h>
#include <fstream>
#include "TrieTree.h"

using namespace std;

int main() {
	clock_t start, finish;
	start = clock();

	TrieTree TrTree;
	// ��dict.txt�е��ַ��������ϣ��
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("dictFiles\\dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			TrTree.addString(str);
		}
	}
	else {
		cout << "�޷����ļ�dict.txt" << endl;
	}
	readStr.close();

	// ��ȡstring.txt���������Ƿ��ڹ�ϣ���У�������д���ļ�reslut.txt
	readStr.open("dictFiles\\string.txt");
	ofstream writeResult;
	writeResult.open("result.txt");
	if (readStr && writeResult) {
		while (readStr.getline(str, TEMP_SIZE)) {
			if (TrTree.hasString(str)) {
				writeResult << str << '\n';
			}
		}
	}
	else {
		cout << "�޷�������ļ�" << endl;
	}
	readStr.close();
	writeResult.close();

	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "����ʱ��:" << duration << 's' << endl;

	return 0;
}