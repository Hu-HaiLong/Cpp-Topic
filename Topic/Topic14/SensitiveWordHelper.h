#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

const int MAX_NODE = 100;
const char NODE_END = '-';

struct Node
{
	Node() 
	{
		Children = (Node**)calloc(MAX_NODE, sizeof(Node));
		for (int i = 0; i < MAX_NODE; ++i)
		{
			Children[i] = (Node*)calloc(MAX_NODE, sizeof(Node));
		}
	}

	~Node()
	{
		delete[] Children;
	}

	char Data = NULL;
	Node** Children = NULL;
};

class SensitiveWordHelper
{
public:
	void Load(const char* FileName);
	bool Check(const char* Words);
	void Print();

private:
	void InitTree(const char* Words);
	Node* Find(Node** Nodes, char Word);
	void Insert(Node** Nodes, Node* Node);
	void PrintChildren(Node** Nodes);

	Node Root;
};

