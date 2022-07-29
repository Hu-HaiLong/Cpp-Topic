#pragma once
#include <iostream>

using namespace std;

extern int CharLen(const char* Str);

class CustomString
{
public:
    CustomString(const CustomString& Str);
    CustomString(const char* Str);
    CustomString();
    ~CustomString();

    CustomString& operator=(const CustomString& Str)
    {
        this->Size = Str.Size;
        Data = new char[this->Size + 1];
        memcpy(this->Data, Str.Data, this->Size * sizeof(char) + 1);
        return *this;
    }

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

    int Len() const;
    CustomString Sub(const int Start, const int End) const;
    void Append(const char* Str);
    int Find(const char* SubStr, int Pos = 0) const;
    CustomString* Split(const char* InS);

private:
    char* Data = nullptr;
    int Size = 0;
};