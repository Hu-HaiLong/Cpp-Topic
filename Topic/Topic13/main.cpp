#include "TabFile.h"

int main()
{
	printf("sad");
	auto Tab = new TabFile();
	Tab->Load("xxxFile.txt");
	auto lineData = Tab->Find("3");  // ÿ�е����ݽṹ�����Լ���һ���̶������ݽṹ

	system("pause");
}