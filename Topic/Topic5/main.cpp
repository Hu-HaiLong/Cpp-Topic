#include <iostream>
#include "RingBuffer.h"
//#include "time.h"

int main()
{
    int InitSize = 256;
    const char* Data = "123456789";
    auto Buffer = new RingBuffer<char>(InitSize);
    srand(time(NULL));
    int Length = strlen(Data) + 1;

    for (int i = 0; i < 1000; i++)
    {
        int PushCount = rand() % 10;

        for (int j = 0; j < PushCount; j++)
        {
            Buffer->Push(Data, Length);    // Ҫ֧���Զ�����
        }

        char Out[128];
        
        int PopCount = rand() % 10;

        for (int j = 0; j < PopCount; j++)
        {
            Buffer->Pop(&Out[0], sizeof(Out));   // Ҫ֧���Զ�����
        }
    }

	return 0;
}