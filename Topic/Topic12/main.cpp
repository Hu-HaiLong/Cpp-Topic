#include <iostream>
#include "PosManager.h"

int main()
{
	auto m = new PosManager();
	int blockSize = 100;
	int mapSize = 10000;
	m->Init(blockSize, mapSize);

	int PoserID1 = 1;
	int PoserID2 = 2;
	m->Add(PoserID1, 100, 200);  // 100�Ǻ����꣬200������
	m->Add(PoserID2, 200, 300);
	int* PoserIDs = m->Find(PoserID1);    // �ҵ�poseid1�����Լ���Χ8�������id
	m->Update(PoserID1, 300, 400);
	m->Remove(PoserID2);
}