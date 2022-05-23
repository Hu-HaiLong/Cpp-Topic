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

CustomString::CustomString()
{
	Data = new char[1];
	*Data = '\0';
	Size = 0;
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

	for (int i = 0; i < Lhs.Size; i++)
	{
		if (Lhs.Data[i] != Rhs.Data[i])
		{
			return false;
		}
	}

	return true;
}

int CustomString::Len() const
{
	return Size;
}

CustomString CustomString::Sub(int Start, int End) const
{
	if (End - Start < 0 || Start < 0)
	{
		return nullptr;
	}

	CustomString* Str = new CustomString("");
	Str->Data = new char[End - Start + 2];
	Str->Size = End - Start + 1;
	memcpy(Str->Data, this->Data + Start, Str->Size * sizeof(char));
	Str->Data[Str->Size] = '\0';
	return *Str;
}

void CustomString::Append(const char* Str)
{
	int oldLen = Size;
	Size += CharLen(Str);
	char* Temp = new char[Size + 1];

	memcpy(Temp, Data, oldLen * sizeof(char) + 1);

	memcpy(Temp + oldLen * sizeof(char), Str, (Size - oldLen) * sizeof(char) + 1);

	delete[] Data;
	Data = Temp;
}

int CustomString::Find(const char* SubStr, int Pos) const
{
	int SubStrLen = CharLen(SubStr);

	for (int L = Pos; L + SubStrLen < Size; L++)
	{
		int R = 0;

		for (; R < SubStrLen; R++)
		{
			if (Data[L+R] != SubStr[R])
			{
				break;
			}
		}

		if (R == SubStrLen)
		{
			return L;
		}
	}

	return -1;
}

CustomString* CustomString::Split(const char* InS)
{
	int Pos1 = 0;
	int Pos2 = Find(InS);
	
	if (Pos2 < 0)
	{
		return this;
	}
	
	CustomString* Strs = new CustomString[Size];
	
	int Num = 0;

	while (Pos2 != -1)
	{
		Strs[Num] = this->Sub(Pos1, Pos2 - Pos1 - 1);
		Pos1 = Pos2 + CharLen(InS);
		Pos2 = this->Find(InS, Pos1);
		Num++;
	}

	if (Pos2 != Size)
	{
		Strs[Num] = this->Sub(Pos1, Size - 1);
	}

	return Strs;
}

extern int CharLen(const char* Str)
{
	int num = 0;

	if (Str == nullptr)
	{
		return num;
	}

	while (*Str != '\0')
	{
		num++;
		Str++;
	}

	return num;
}