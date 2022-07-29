#pragma once
#include "stdlib.h"
#include <string> 

#define MAX_SIZE (1 << 16)

struct BufferInfo {
    BufferInfo(int R = 0, int W = 0, int S = 0) : Read(R), Write(W), Size(S) {}

    int Read;
    int Write;
    int Size;
};

template <typename T>
class RingBuffer
{
public:
    RingBuffer(int BufSize = 0) : Buffer(nullptr)
    {
        Create(BufSize);
    }

    ~RingBuffer()
    {
        Destroy();
    }

    void Push(const T* Data, int Len)
    {
        if (Len > GetCanWriteCount())
        {
            bool Result = ExpandSize(Len);

            if (!Result)
            {
                return;
            }
        }

        memcpy((Buffer + BufferInfo.Write * sizeof(T)), Data, Len * sizeof(T));
        BufferInfo.Write += Len;
    }

    void Pop(T* Buf, int Len)
    {
        int Count = 0;

        while (!IsEmpty())
        {
            Buf[Count++] = Buffer[BufferInfo.Read * sizeof(T)];
            Pop(1);

            if (Count >= Len)
            {
                break;
            }
        }

        if (IsEmpty())
        {
            ReduceSize();
        }
    }

    void Pop(int Num)
    {
        int Count = GetCanReadCount();

        if (Num < Count)
        {
            BufferInfo.Read += Num;
            return;
        }

        BufferInfo.Read = BufferInfo.Write = 0;
    }

private:

    bool Create(int BufSize)
    {
        if (BufSize == 0)
        {
            return false;
        }

        if (Buffer != nullptr)
        {
            delete[] Buffer;
            Buffer = nullptr;
        }

        int Length = BufSize;
        int NewLen = 1;

        while (Length > NewLen)
        {
            NewLen *= 2;

            if (NewLen > MAX_SIZE)
            {
                return false;
            }
        }

        Buffer = new T[NewLen];
        BufferInfo.Read = 0;
        BufferInfo.Write = 0;
        BufferInfo.Size = NewLen;

        return true;
    }

    void Destroy()
    {
        if (Buffer == nullptr)
        {
            return;
        }

        delete[] Buffer;
        Buffer = nullptr;
    }

    bool ExpandSize(int InLen = 2)
    {
        T* Temp = new T[BufferInfo.Size];
        memcpy(Temp, Buffer, BufferInfo.Size * sizeof(T));

        if (Buffer != nullptr)
        {
            delete[] Buffer;
            Buffer = nullptr;
        }

        int Size = BufferInfo.Size * 2;

        while (InLen > Size)
        {
            Size *= 2;

            if (Size > MAX_SIZE)
            {
                return false;
            }
        }

        Buffer = new T[Size];
        memcpy(Buffer, Temp, BufferInfo.Size * sizeof(T));
        BufferInfo.Size = Size;

        return true;
    }

    bool ReduceSize()
    {
        if (!IsEmpty() || IsMinSize())
        {
            return false;
        }

        if (Buffer != nullptr)
        {
            delete[] Buffer;
            Buffer = nullptr;
        }

        int Length = BufferInfo.Size / 2;
        Buffer = new T[Length];
        BufferInfo.Size = Length;

        return true;
    }

    inline bool IsFull() const
    {
        return GetCanWriteCount() <= 0;
    }

    inline bool IsEmpty() const
    {
        return GetCanReadCount() == 0;
    }

    inline int IsMinSize() const
    {
        return BufferInfo.Size <= 2;
    }

    inline int GetCanReadCount() const
    {
        return BufferInfo.Write - BufferInfo.Read;
    }

    inline int GetCanWriteCount() const
    {
        return BufferInfo.Size - BufferInfo.Write - 1;
    }

private:
    T* Buffer;
    BufferInfo BufferInfo = NULL;
};

