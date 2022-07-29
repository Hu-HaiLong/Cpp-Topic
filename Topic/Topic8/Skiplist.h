#pragma once
#include <iostream>
#include <vector>

using namespace std;

const int MAX_LEVEL = 3;
const float RANDS = 0.5;

struct Node
{
    Node(int Value = -1, int Size = MAX_LEVEL) : Value(Value), Right(Size) {}

    vector<Node*> Right;
    int Value;
};

class Skiplist
{
public:
    Skiplist()
    {
        srand((unsigned)time(NULL));
        Head = new Node(-1, MAX_LEVEL);
    }

    ~Skiplist()
    {
        delete[] Head;
        Head = nullptr;
    }

    bool Search(int Target) const
    {
        vector<Node*> Prevs = FindPrevNodes(Target);
        return Prevs[0]->Right[0] && Prevs[0]->Right[0]->Value == Target;
    }

    void Add(int Num)
    {
        vector<Node*> Prevs = FindPrevNodes(Num);

        int Level = GetLevel();

        if (Level > MaxLevel)
        {
            for (int i = MaxLevel; i < Level; i++)
            {
                Prevs[i] = Head;
            }

            MaxLevel = Level;
        }

        Node* NewNode = new Node(Num, Level);

        for (int i = Level - 1; i >= 0; i--)
        {
            NewNode->Right[i] = Prevs[i]->Right[i];
            Prevs[i]->Right[i] = NewNode;
        }
    }

    bool Erase(int Target)
    {
        vector<Node*> Prevs = FindPrevNodes(Target);

        if (Prevs[0]->Right[0] || Prevs[0]->Right[0]->Value != Target)
        {
            return false;
        }

        Node* DelNode = Prevs[0]->Right[0];

        for (int i = 0; i < MaxLevel; i++)
        {
            if (Prevs[i]->Right[i] == DelNode)
            {
                Prevs[i]->Right[i] = DelNode->Right[i];
            }
        }

        delete DelNode;

        // 更新MaxLevel
        while (MaxLevel > 1 && !Head->Right[MaxLevel - 1])
        {
            MaxLevel--;
        }

        return true;
    }

public:
    void Print() const
    {
        for (int Level = 0; Level < MAX_LEVEL; Level++)
        {
            cout << " Level : " << Level << '\n';

            Node* Temp = Head;

            while (Temp->Right[Level] && Temp->Right[Level]->Value != -1)
            {
                cout << Temp->Right[Level]->Value << '\t';
                Temp = Temp->Right[Level];
            }

            cout << '\n';
        }
    }


private:
    vector<Node*> FindPrevNodes(int Target) const
    {
        Node* Temp = Head;
        vector<Node*> Prevs(MAX_LEVEL);

        for (int i = MaxLevel - 1; i >= 0; i--)
        {
            while (Temp->Right[i] && Temp->Right[i]->Value < Target)
            {
                Temp = Temp->Right[i];
            }

            Prevs[i] = Temp;
        }

        return Prevs;
    }

    // 生成并获取一个节点的level
    int GetLevel()
    {
        int Level = 1;

        while (rand01() < RANDS && Level < MAX_LEVEL)
        {
            Level += 1;
        }

        return Level;
    }

    // Random number between [0,1]
    float rand01()
    {
        return (1 * rand()) * (1.f / RAND_MAX);
    }

private:
    Node* Head = nullptr;

    int MaxLevel = 1;
};