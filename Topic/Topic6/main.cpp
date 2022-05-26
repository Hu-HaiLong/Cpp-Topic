#include <iostream>
#include "StackInfo.h"

class Test
{
public:
	int P1 = 1;
	bool P2 = false;
	std::string P3 = "p33333333";

	void TestWithStack(StackInfo& Stack)
	{
		int V1 = 0;
		std::string V2;
		Test V3;
		Test* V4 = nullptr;

		Stack.Pop<Test*>(V4);
		Stack.Pop(V3);
		Stack.Pop(V2);
		Stack.Pop(V1);

		std::cout << "TestWithStack "
			<< V1 << " "
			<< V2 << " "
			<< V3.P3 << " "
			<< V4->P3 << std::endl;

		Stack.Push(1);
	}
};

void TestFunc()
{
	Test T1;
	StackInfo Stack;  // 实现此类，就俩模板函数，Push和Pop
	Stack.Push(111);
	Stack.Push<std::string>("test string");

	Test T2;
	T2.P3 = "t2teststring";
	Stack.Push(T2);
	Stack.Push(&T1);

	T1.TestWithStack(Stack);

	int Ret = 0;
	Stack.Pop(Ret);
	std::cout << "TestWithStack Ret " << Ret << std::endl;
}

int main()
{
	TestFunc();
	return 0;
}