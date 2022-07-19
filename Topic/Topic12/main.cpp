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
	m->Add(PoserID1, 100, 200);  // 100是横坐标，200纵坐标
	m->Add(PoserID2, 200, 300);
	int* PoserIDs = m->Find(PoserID1);    // 找到poseid1脚下以及周围8格的所有id
	m->Update(PoserID1, 300, 400);
	m->Remove(PoserID2);
}