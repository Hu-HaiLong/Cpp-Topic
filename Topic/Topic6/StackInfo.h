#pragma once

struct Node
{
	void* Data = nullptr;
	Node* Next = nullptr;
};

class StackInfo
{
public:
	~StackInfo()
	{
		Node* Temp = Top;
		while (Temp)
		{
			Temp = Top->Next;
			delete Top->Data;
			delete Top;
		}
		delete Temp;
		Temp = nullptr;
		Top = nullptr;
	}

	template <typename T>
	void Push(const T& t);

	template <typename T>
	void Pop(T& t);

private:
	Node* Top = nullptr;
};

template <typename T>
void StackInfo::Push(const T& Value)
{
	T* P = new T(Value);
	Node* NewOne = new Node();
	NewOne->Data = (void*)P;

	if (Top == nullptr)
	{
		Top = NewOne;
		return;
	}

	NewOne->Next = Top;
	Top = NewOne;
}

template <typename T>
void StackInfo::Pop(T& Data)
{
	if (Top == nullptr)
	{
		return;
	}

	Node* Temp = Top->Next;
	Data = *(T*)Top->Data;
	delete Top->Data;
	delete Top;
	Top = Temp;
}