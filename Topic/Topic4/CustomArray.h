#pragma once
#include <iostream>
#include <cstdio>
#include <stdio.h>
using namespace std;

template <typename T = int>
class CustomArray
{
private:
	int Cur = -1;
	int Size;
	T* KArray;

public:
	CustomArray(const CustomArray& Arr)
	{
		this->Cur = Arr.Cur;
		this->Size = Arr.Size;
		this->KArray = new T[this->Size];
		memcpy(this->KArray, Arr.KArray, this->Size * sizeof(T) + 1);
	}

	CustomArray(int Size = 0)
	{
		this->Size = Size;
		KArray = new T[Size];
	}

	~CustomArray()
	{
		delete[] KArray;
		KArray = nullptr;
		Size = 0;
		Cur = -1;
	}

	void Reserve(int Size)
	{
		if (KArray != nullptr)
		{
			delete[] KArray;
		}
		KArray = new T[Size];
		this->Size = Size;
	}

	void Push(const T& Value)
	{
		if (Cur == Size)
		{
			return;
		}

		KArray[++Cur] = Value;
	}
	
	void Insert(int Pos, const T& Value)
	{
		if (Pos < 0 || Pos - 1 >= Size || Pos - 1 > Cur)
		{
			return;
		}

		for (int i = Size - 1; i >= Pos; i--)
		{
			KArray[i] = KArray[i - 1];
		}

		KArray[Pos - 1] = Value;
		Cur++;
	}
	
	void Remove(int Pos)
	{
		if (Cur == -1)
		{
			return;
		}

		if (Pos < 0 || Pos - 1 >= Size || Pos - 1 > Cur)
		{
			return;
		}

		for (int i = Pos - 1; i <= Cur; i++)
		{
			KArray[i] = KArray[i + 1];
		}

		Cur--;
	}
	
	void Pop()
	{
		Remove(1);
	}
	
	void Clear()
	{
		if (Cur == -1)
		{
			return;
		}

		for (int i = 0; i <= Cur; i++)
		{
			KArray[i] = NULL;
		}

		Cur = -1;
	}
	
	int FindIndex(const T& Value)
	{
		if (Cur == -1)
		{
			return -1;
		}

		for (int i = 0; i <= Cur; i++)
		{
			if (KArray[i] == Value)
			{
				return i;
			}
		}

		return -1;
	}

public:
	void Print()
	{
		if (Cur == -1)
		{
			cout << "Print Array: Null" << '\n';
			return;
		}

		cout << "Print Array:" << '\n';

		for (int i = 0; i <= Cur; i++)
		{
			cout << KArray[i] << '\n';
		}
	}
};
