#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

struct TabData
{
    int ID; // Key
    string Name;
    bool bIsCan;
};

class TabFile
{
public:
    void Load(const char* FileName);
    TabData Find(const char* Key);

private:
    void WriteInt(string Value, int& DataOffset);
    void WriteString(string Value, int& DataOffset);
    void WriteBool(string Value, int& DataOffset);

    void Write(const char* Value, int Len);

    string CompStr2Number(string Value, int& CompStr2Number);
    string CompStr(string Value, int& CompStr2Number);
    string UnCompStr(const char* Value, int Size);

    int ReadInt(int Offset);
    bool ReadBool(int Offset);
    string ReadStr(int Offset, int Key);

    int Size = 0;
    char* Buffer; // 写入

    map<int, int> Offset; // 每Key数据偏移头
    map<int, int> OffsetLast; // 每Key数据偏移尾
    map<string, int> StrSameData; // Str相同值  <value，offset>
};
