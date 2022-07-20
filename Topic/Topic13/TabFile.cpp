#include "TabFile.h"

void TabFile::Load(const char* FileName)
{
    ifstream Filestream(FileName);

    string Line;

    while (getline(Filestream, Line))
    {
        string Key, X, Y;
        stringstream LineStream;

        LineStream << Line;
        LineStream >> Key >> X >> Y;
        
        Data[Key] = Line;

        cout << Key << X << Y;
    }

    Filestream.close();
}

string TabFile::Find(const char* Key)
{
    return Data[Key];
}
