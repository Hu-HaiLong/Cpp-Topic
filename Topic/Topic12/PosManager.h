#pragma once

const int MAX_POSER = 100; // 一个格子最大多少人

class PosManager
{
public:
    PosManager() {};
    ~PosManager()
    {
        delete[] Map;
    };

    void Init(int BlockSize, int MapSize);
    void Add(int PoserID, int X, int Y);
    int* Find(int PoserID);
    void Update(int PoserID, int X, int Y);
    void Remove(int PoserID);

private:
    int GetBlockID(int InX, int InY);
    int InsertPoser(int* Poser, int PoserID);
    int FindPoser(int* Poser, int PoserID);
    bool FindPoserPos(int& X, int& Y, int& BlockID, int PoserID);

    int** Map = nullptr;
    int BlockSize = 0;
    int MapSize = 0;
};

