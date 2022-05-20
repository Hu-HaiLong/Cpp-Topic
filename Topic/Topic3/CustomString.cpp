#include "CustomString.h"

CustomString::CustomString(const CustomString& Str)
{
	this->Size = Str.Size;
	Data = new char[this->Size + 1];
	memcpy(this->Data, Str.Data, this->Size * sizeof(char) + 1);
}

CustomString::CustomString(const char* Str)
{
	if (Str == nullptr)
	{
		Data = new char[1];
		*Data = '\0';
		Size = 0;
	}
	else
	{
		Size = CharLen(Str);
		Data = new char[Size + 1];
		memcpy(Data, Str, Size * sizeof(char) + 1);
	}
}

CustomString::~CustomString()
{
	if (Data != nullptr)
	{
		delete[] Data;
		Data = nullptr;
		Size = 0;
	}
}

bool operator==(const CustomString& Lhs, const CustomString& Rhs)
{
	if (Lhs.Size != Rhs.Size)
	{
		return false;
	}

	for (size_t i = 0; i < Lhs.Size; i++)
	{
		if (Lhs.Data[i] != Rhs.Data[i])
		{
			return false;
		}
	}

	return true;
}

size_t CustomString::Len()
{
	return Size;
}

char* CustomString::Sub(int Start, int End)
{
	char* Temp = new char[End - Start + 2];

	for (int i = Start; i <= End; i++)
	{
		Temp[i - Start] = Data[i];
	}

	size_t TempSize = End - Start + 1;
	Temp[TempSize] = '\0';
	return Temp;
}

void CustomString::Append(const char* Other)
{
	size_t oldLen = Size;
	Size += CharLen(Other);
	char* Temp = new char[Size + 1];

	for (size_t i = 0; i < oldLen; i++)
	{
		Temp[i] = Data[i];
	}

	delete[] Data;

	for (size_t j = oldLen; j < Size; j++)
	{
		Temp[j] = Other[j - oldLen];
	}

	Temp[Size] = '\0';
	Data = Temp;
}

int CustomString::Find(const char* Other)
{
	size_t OtherLen = CharLen(Other);

	for (size_t L = 0; L + OtherLen < Size; L++)
	{
		size_t R = 0;

		for (; R < OtherLen; R++)
		{
			if (Data[L+R] != Other[R])
			{
				break;
			}
		}

		if (R == OtherLen)
		{
			return L;
		}
	}

	return -1;
}

CustomString* CustomString::Split(const char* InS)
{
	int InPos = Find(InS);
	
	if (InPos < 0)
	{
		return nullptr;
	}

	Data = Sub(0, InPos - 1);
	Size = InPos;

	return this;
}