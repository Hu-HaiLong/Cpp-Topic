#include <iostream>
#include "CustomString.h"
using namespace std;

int main()
{
    auto Str1 = CustomString("test1");
    auto Str2 = CustomString("test2,test3");
    auto Str3 = Str2;

    Str1 = "test3";

    int Len = Str1.Len();

    CustomString Sub1 = Str1.Sub(1, 2);

    Str1.Append("append");

    bool bEqual = Str1 == Str2;

    int Index = Str1.Find("es");

    CustomString* Ret = Str2.Split(",");

    return 0;
}