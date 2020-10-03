// 实验一.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include <fstream>
#include <time.h>
#include "HashTable.h"


using namespace std;

int main()
{
	clock_t start,finish;
	start = clock();

	HashTable hashTable;
	// 将dict.txt中的字符串存入哈希表
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("dictFiles/dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			hashTable.addString(str);
		}
	}
	else {
		cout << "无法打开文件dict.txt" << endl;
	}
	readStr.close();
	// 读取string.txt，逐个检测是否在哈希表中，若在则写入文件reslut.txt
	readStr.open("dictFiles/string.txt");
	ofstream writeResult;
	writeResult.open("result.txt");
	if (readStr && writeResult) {
		while (readStr.getline(str, TEMP_SIZE)) {
			if (hashTable.hasString(str)) {
				writeResult << str << '\n';
			}
		}
	}
	else {
		cout << "无法打开输出文件" << endl;
	}
	readStr.close();
	writeResult.close();

	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "程序运行时间:" << duration << 's' << endl;

	return 0;
}
