#include <iostream>
#include <time.h>
#include <fstream>
#include "MTrieTree.h"

using namespace std;

int main() {
	clock_t start, finish;
	start = clock();

	// 树分多少叉
	MTrieTree MTrTree(2);
	// 将dict.txt中的字符串存入哈希表
	ifstream readStr;
	const int TEMP_SIZE = 200;
	char str[TEMP_SIZE];
	readStr.open("dictFiles\\dict.txt");
	if (readStr) {
		while (readStr.getline(str, TEMP_SIZE)) {
			MTrTree.addString(str);
		}
	}
	else {
		cout << "无法打开文件dict.txt" << endl;
	}
	readStr.close();

	// 读取string.txt，逐个检测是否在哈希表中，若在则写入文件reslut.txt
	readStr.open("dictFiles\\string.txt");
	ofstream writeResult;
	writeResult.open("result.txt");
	if (readStr && writeResult) {
		while (readStr.getline(str, TEMP_SIZE)) {
			if (MTrTree.hasString(str)) {
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
	cout << "运行时间:" << duration << 's' << endl;


	return 0;
}