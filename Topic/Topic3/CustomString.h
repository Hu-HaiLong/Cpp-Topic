#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <stdio.h>
using namespace std;

class CustomString
{
private:
	char* Data;
	size_t Size;

public:
	CustomString(const CustomString& Str);
	CustomString(const char* Str);
	~CustomString();

	CustomString& operator=(const char* Str)
	{
		if (Data != nullptr)
		{
			delete[] Data;
		}

		Size = (Str && *Str) ? CharLen(Str) : 0;
		Data = new char[Size + 1];

		if (Size != 0)
		{
			memcpy(Data, Str, Size * sizeof(char) + 1);
		}
		else
		{
			*Data = '\0';
		}

		return *this;
	}

	friend bool operator==(const CustomString& Lhs, const CustomString& Rhs);

	size_t CharLen(const char* Str)
	{
		size_t num = 0;

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

	size_t Len();
	char* Sub(const int Start, const int End);
	void Append(const char* Other);
	int Find(const char* Other);
	CustomString* Split(const char* InS);
};