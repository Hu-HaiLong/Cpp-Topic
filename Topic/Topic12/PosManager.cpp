#include "PosManager.h"
#include <malloc.h>
#include <string.h>

void PosManager::Init(int InBlockSize, int InMapSize)
{
    MapSize = InMapSize;
    BlockSize = InBlockSize;
    int Size = MapSize / BlockSize;
    
    Map = (int**)malloc(sizeof(int*) * Size * Size);

    for (int i = 0; i < Size * Size; i++)
    {
        Map[i] = (int*)malloc(sizeof(int) * MAX_POSER);
        for (int j = 0; j <= MAX_POSER; j++)
        {
            Map[i][j] = -1;
        }
    }
}

void PosManager::Add(int PoserID, int InX, int InY)
{
    if (InX > MapSize || InY > MapSize)
    {
        return;
    }

    int X = InX / BlockSize;
    int Y = InY / BlockSize;
    int MapID = GetBlockID(X, Y);
    InsertPoser(Map[MapID], PoserID);
}

int* PosManager::Find(int PoserID)
{
    int* Posers = new int[9];
    int X = -1;
    int Y = -1;
    int BlockID = 0;

    FindPoserPos(X, Y, BlockID, PoserID);
    
    if (X == -1 || Y == -1)
    {
        return Posers;
    }

    int Count = 0;
    
    for (int i = X - 1; i <= X + 1; i++)
    {
        if (i < 0 || i > MapSize / BlockSize)
        {
            continue;
        }

        for (int j = Y - 1; j <= Y + 1; j++)
        {
            if (j < 0 || j > MapSize / BlockSize)
            {
                continue;
            }

            BlockID = GetBlockID(X, Y);

            if (Map[BlockID][0] < 0)
            {
                continue;
            }

            for (int l = 0; l < MAX_POSER; l++)
            {
                if (Map[BlockID][l] > -1)
                {
                    memcpy(&Posers[Count++], &Map[i][j], sizeof(int));
                }
            }
        }
    }

    return Posers;
}

void PosManager::Update(int PoserID, int InX, int InY)
{
    Remove(PoserID);
    Add(PoserID, InX, InY);
}

void PosManager::Remove(int PoserID)
{
    int BlockID = -1;
    int Index = -1;

    bool bBreakLoop = true;

    for (int i = 0; i < MapSize / BlockSize && bBreakLoop; i++)
    {
        for (int j = 0; j < MapSize / BlockSize && bBreakLoop; j++)
        {
            BlockID = GetBlockID(i, j);

            Index = FindPoser(Map[BlockID], PoserID);

            if (Index != -1)
            {
                bBreakLoop = false;
            }
        }
    }
    
    if (Index == -1)
    {
        return;
    }

    for (int i = Index; i < MAX_POSER; i++)
    {
        Map[BlockID][i] = Map[BlockID][i + 1];
    }
}

int PosManager::GetBlockID(int InX, int InY)
{
    return InX * 100 + InY;
}

int PosManager::InsertPoser(int* Poser, int PoserID)
{
    int i = 0;
    
    for (i = 0; i < MAX_POSER; i++)
    {
        if (Poser[i] == -1)
        {
            break;
        }
    }

    Poser[i] = PoserID;

    return i;
}

int PosManager::FindPoser(int* Poser, int PoserID)
{
    for (int i = 0; i < MAX_POSER; i++)
    {
        if (Poser[i] == PoserID)
        {
            return i;
        }
    }

    return -1;
}

bool PosManager::FindPoserPos(int& X, int& Y, int& BlockID, int PoserID)
{
    for (int i = 0; i < MapSize / BlockSize; i++)
    {
        for (int j = 0; j < MapSize / BlockSize; j++)
        {
            BlockID = GetBlockID(i, j);

            if (FindPoser(Map[BlockID], PoserID) == -1)
            {
                continue;
            }

            X = i;
            Y = j;
            return true;
        }
    }

    return false;
}