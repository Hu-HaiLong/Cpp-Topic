#include "TabFile.h"
#include <vector>

void TabFile::Load(const char* FileName)
{
    ifstream Filestream(FileName);

    string Line;

    int LineNum = 0;

    while (getline(Filestream, Line))
    {
        string Key, Name, Price, bIsCan;
        stringstream LineStream;

        LineStream << Line;
        LineStream >> Key >> Name >> Price >> bIsCan;

        cout << Key << Name << Price << bIsCan;

        Offset[atoi(Key.c_str())] = Size;
        
        int DataOffset = Size;
        WriteInt(Key, DataOffset);
        WriteBool(bIsCan, DataOffset);
        WriteString(Name, DataOffset);

        OffsetLast[atoi(Key.c_str())] = Size;
    }

    Filestream.close();
}

TabData TabFile::Find(const char* Key)
{
    TabData Tab;
    Tab.ID = ReadInt(Offset[atoi(Key)]);
    Tab.Name = ReadStr(Offset[atoi(Key)], atoi(Key));
    Tab.bIsCan = ReadBool(Offset[atoi(Key)]);
    return Tab;
}

void TabFile::WriteInt(string Value, int& DataOffset)
{
    int a = atoi(Value.c_str());

    char* buf = new char[4];
    buf[0] = a >> 24;
    buf[1] = a >> 16;
    buf[2] = a >> 8;
    buf[3] = a;

    Write(buf, 4);
    DataOffset += 4;

    delete[] buf;
    buf = nullptr;
}

void TabFile::WriteString(string Value, int& DataOffset)
{
    string v = Value;

    int StrSize = 0;

    auto Iter = StrSameData.find(Value);
    
    if (Iter != StrSameData.end())
    {
        // 有相同值 拼接同志的偏移
        int Offset = Iter->second;
        string Tmp = "1" + to_string(Offset);
        v = Tmp.c_str();
        StrSize = strlen(v.c_str());
    }
    else
    {
        StrSameData[Value] = DataOffset;
        // 非相同值 非默认值
        string Tmp = "2";
        Tmp.append(Value);
        Tmp = CompStr2Number(Tmp, *(&StrSize));
        v = Tmp.c_str();
        DataOffset += StrSize;

        return;
    }

    if (Value == "DEFAULT")
    {
        // 默认换成0
        v = "0";
        StrSize = 1;
    }

    Write(v.c_str(), StrSize);

    DataOffset += StrSize;
}

void TabFile::WriteBool(string Value, int& DataOffset)
{
    const char* v;
    
    if (Value == "true")
    {
        v = "1";
    }
    else
    {
        v = "0";
    }

    Write(v, 0);

    DataOffset += strlen(v);
}

void TabFile::Write(const char* Value, int Len = 0)
{
    int OldLen = Size;
    Size += Len == 0 ? strlen(Value) : Len;
    char* Temp = new char[Size + 1];
    memset(Temp, 0, sizeof(char) * (Size + 1));

    if (!Buffer)
    {
        Buffer = new char[Size + 1];
    }
    else
    {
        memcpy(Temp, Buffer, OldLen * sizeof(char) + 1);

        delete[] Buffer;
        Buffer = nullptr;

        Buffer = new char[Size + 1];
    }

    memcpy(Temp + OldLen * sizeof(char), Value, (Size - OldLen) * sizeof(char) + 1);
    memcpy(Buffer, Temp, Size * sizeof(char) + 1);

    if (Temp)
    {
        delete[] Temp;
        Temp = nullptr;
    }
}

string TabFile::CompStr2Number(string Value, int& CompStr2Number)
{
    int i = 1;
    string Tmp(1, Value[0]);  // 首位不转

    while (Value[i] != '\0') 
    {
        string ASC = to_string(Value[i]);
        
        if (ASC.length() == 1)
        {
            ASC = "00" + ASC;
        }
        else if (ASC.length() == 2)
        {
            ASC = "0" + ASC;
        }

        Tmp.append(ASC);

        i++;
    }

    Tmp = CompStr(Tmp, CompStr2Number);

    return Tmp;
}

string TabFile::CompStr(string Value, int& CompStr2Number)
{
    int i = 0;

    vector<string> Res;

    while (Value.length() > 4)
    {

        string temp = Value.substr(0, 4);
        Res.push_back(temp);
        //去掉已分割的字符串,在剩下的字符串中进行分割
        Value = Value.substr(4, Value.length());
    }
    Res.push_back(Value);

    char* buf = new char[4 * Res.size() + 1];
    //memset(buf, 0, sizeof(char) * 4 * Res.size());

    for (int j = 0; j < Res.size(); j++)
    {
        int Tmp = atoi(Res[j].c_str());

        buf[j * 4 + 0] = Tmp >> 24;
        buf[j * 4 + 1] = Tmp >> 16;
        buf[j * 4 + 2] = Tmp >> 8;
        buf[j * 4 + 3] = Tmp;
    }
    CompStr2Number = 4 * Res.size();

    buf[4 * Res.size()] = '\0';

    Write(buf, CompStr2Number);

    string Tmp = buf;

    delete[] buf;
    buf = nullptr;

    return Tmp;
}

string TabFile::UnCompStr(const char* Value, int Size)
{
    string Tmp1;

    for (int i = 0; i < Size; i += 4)
    {
        int num = Value[i + 3] & 0xFF;
        num |= ((Value[i + 2] << 8) & 0xFF00);
        num |= ((Value[i + 1] << 16) & 0xFF0000);
        num |= ((Value[i + 0] << 24) & 0xFF0000);

        if (num > 100 and num < 1000)
        {
            Tmp1.append("0");
        }
        
        Tmp1.append(to_string(num));
    }
 
    return Tmp1;
}

int TabFile::ReadInt(int Offset)
{
    int Num = Buffer[Offset + 3] & 0xFF;
    Num |= ((Buffer[Offset + 2] << 8) & 0xFF00);
    Num |= ((Buffer[Offset + 1] << 16) & 0xFF0000);
    Num |= ((Buffer[Offset + 0] << 24) & 0xFF0000);

    return Num;
}

bool TabFile::ReadBool(int Offset)
{
    char Tmp = Buffer[Offset + 4];
    
    if (Tmp == '0')
    {
        return true;
    }

    return false;
}

string TabFile::ReadStr(int OffsetNum, int Key)
{
    int Size = OffsetLast[Key];
    char* Tmp = new char[(Size - OffsetNum - 5) * sizeof(char) + 1];
    memcpy(Tmp, &Buffer[OffsetNum + 5], (Size - OffsetNum - 5) * sizeof(char));
    Tmp[Size - OffsetNum - 5] = '\0';

    string Tmp1 = UnCompStr(Tmp, Size - OffsetNum - 5);

    delete[] Tmp;
    Tmp = nullptr;

    if (Tmp1[0] == '0')
    {
        Tmp1 = "DEFAULT";
    }
    else if (Tmp1[0] == '2')
    {
        Tmp1 = Tmp1.substr(1, Tmp1.size());

        int Size = (Tmp1.size() - 1) / 3;

        string Tmp2;

        for (int i = 0; i <= Size; i++)
        {
            int Tmp3 = atoi(Tmp1.substr(i * 3, 3).c_str());
            Tmp2 += char(Tmp3);
        }

        Tmp1 = Tmp2;
    }

    return Tmp1;
}

