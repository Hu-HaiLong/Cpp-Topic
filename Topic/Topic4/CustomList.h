#pragma once
#include <iostream>
#include <cstdio>
#include <stdio.h>
using namespace std;

template <typename P>
class Node
{
public:
	Node() {}

	Node* Pre;
	Node* Next;
	
	void SetData(P Data) {
		this->Data = Data;
	}

	P GetData() 
	{
		return this->Data;
	}

private:
	P Data;
};

template <typename T = int>
class CustomList
{
private:
	int Size = 0;
	Node<T>* Head;

public:
	CustomList(const CustomList& List)
	{
		this->Size = List.Size;
		memcpy(this->Head, List.Head, this->Size * sizeof(T) + 1);
	}

	CustomList()
	{
		Head = new Node<T>();
		Head->Pre = NULL;
		Head->Next = NULL;
		Head->SetData(NULL);
	}

	~CustomList() 
	{
		PopAll();
	}

	Node<T>* Push(T Data)
	{
		Node<T>* NewOne = new Node<T>();
		NewOne->SetData(Data);
		
		if (Size == 0) {
			Head->Next = NewOne;
			NewOne->Pre = Head;
			Size++;
			return NewOne;
		}

		NewOne->Next = Head->Next;
		NewOne->Pre = Head;
		Head->Next->Pre = NewOne;
		Head->Next = NewOne;

		Size++;
		return NewOne;
	}

	Node<T>* Insert(Node<T>* Pos, T Value)
	{
		if (Pos == nullptr)
		{
			return nullptr;
		}
		Node<T>* PreNode = new Node<T>();
		Node<T>* NewOne = new Node<T>();

		PreNode = Pos->Pre;
		NewOne->Next = Pos;
		Pos->Pre = NewOne;
		PreNode->Next = NewOne;
		NewOne->Pre = PreNode;
		NewOne->SetData(Value);

		Size++;
		return NewOne;
	}

	Node<T>* Find(const T& Data)
	{
		Node<T>* CurNode = Head;
		int CurCount = 0;

		while (CurCount <= Size)
		{
			if (CurNode->GetData() == Data)
			{
				return CurNode;
			}
			CurNode = CurNode->Next;
			CurCount++;
		}

		return nullptr;
	}

	void Remove(Node<T>* Node)
	{
		if (Node == nullptr)
		{
			return;
		}

		Node->Pre->Next = Node->Next;
		Node->Next->Pre = Node->Pre;

		Node->Next = Node->Pre = nullptr;
		delete Node;
		Size--;	
	}

	void PopAll()
	{
		Node<T>* Temp = new Node<T>();

		while (Head != nullptr)
		{
			Temp = Head->Next;
			delete Head;
			Head = Temp;
		}

		Size = 0;
	}

	Node<T>* GetNode(int Index) {
		Node<T>* CurNode = Head;
		int CurCount = 0;

		while (CurCount < Index)
		{
			CurNode = CurNode->Next;
			CurCount++;
		}

		return CurNode;
	}
};

