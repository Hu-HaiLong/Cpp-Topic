#pragma once
#include <iostream>

using namespace std;

template <typename T = int>
class CustomArray
{
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
		T* Temp = new T[this->Size];
		memcpy(Temp, KArray, this->Size * sizeof(T));
		
		if (KArray != nullptr)
		{
			delete[] KArray;
		}

		KArray = new T[Size];
		memcpy(KArray, Temp, this->Size * sizeof(T));
		this->Size = Size;
		this->Cur = Cur;
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
		if (Pos < 0 || Pos >= Size)
		{
			return;
		}

		for (int i = Cur + 1; i >= Pos; i--)
		{
			memcpy(&KArray[i], &KArray[i - 1], sizeof(T));
		}

		KArray[Pos] = Value;
		Cur++;
	}
	
	void Remove(int Pos)
	{
		if (Cur == -1)
		{
			return;
		}

		if (Pos < 0 || Pos > Cur)
		{
			return;
		}

		for (int i = Pos; i <= Cur; i++)
		{
			memcpy(&KArray[i], &KArray[i + 1], sizeof(T));
		}

		Cur--;
	}
	
	T Pop()
	{
		T Result = KArray[0];
		Remove(0);
		return Result;
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

private:
	int Cur = -1;
	int Size = 0;
	T* KArray = nullptr;
};
