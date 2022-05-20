#include <iostream>
#include "CustomArray.h"
#include "CustomList.h"

int main()
{
	//实现数组以及一下api
	auto Arr = new CustomArray<>;
	Arr->Reserve(4); 
	Arr->Push(1);
	Arr->Push(2);
	Arr->Insert(1, 3);
	Arr->Print();
	Arr->Remove(1);
	Arr->Pop();
	Arr->Clear();
	Arr->Print();
	int Index = Arr->FindIndex(1);
	Arr->Print();

	//实现双向链表以下api
	auto list1 = new CustomList<>;
	auto node1 = list1->Push(1);
	auto node2 = list1->Push(2);
 	auto node3 = list1->Insert(node2, 3);
	auto ret = list1->Find(2);
	list1->Remove(node2);
	list1->PopAll();
	
	return 0;
}