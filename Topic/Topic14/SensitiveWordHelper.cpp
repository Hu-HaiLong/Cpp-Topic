#include "SensitiveWordHelper.h"


void SensitiveWordHelper::Load(const char* FileName)
{
    std::ifstream Filestream(FileName);

    std::string Line;

    while (std::getline(Filestream, Line))
    {
        InitTree(Line.c_str());
    }

    Filestream.close();
}

bool SensitiveWordHelper::Check(const char* Words)
{
    Node* CurNode = &Root;

    while (*Words != '\0')
    {
        Node* NewNode = Find(CurNode->Children, *Words);

        if (NewNode != nullptr)
        {
            CurNode = NewNode;
        }
        else
        {
            CurNode = &Root;
        }

        if (Find(CurNode->Children, NODE_END) != nullptr)
        {
            return true;
        }
        
        Words++;
    }
    
    return false;
}

void SensitiveWordHelper::InitTree(const char* Words)
{
    Node* CurNode = &Root;
    
    while (*Words != '\0')
    {
        Node* NewNode = Find(CurNode->Children, *Words);
        
        if (NewNode == nullptr)
        {
            NewNode = new Node();
            NewNode->Data = *Words;
            Insert(CurNode->Children, NewNode);
        }

        CurNode = NewNode;
        Words++;
    }

    Node* NewNode = new Node();
    NewNode->Data = NODE_END;
    Insert(CurNode->Children, NewNode);
}

Node* SensitiveWordHelper::Find(Node** Nodes, char Word)
{
	if (Nodes == nullptr)
	{
		return nullptr;
	}

	for (int i = 0; i < MAX_NODE; i++)
	{
		if ((*Nodes)[i].Data == Word)
		{
			return &(*Nodes)[i];
		}
	}

	return nullptr;
}

void SensitiveWordHelper::Insert(Node** Nodes, Node* Node)
{
    for (int i = 0; i < MAX_NODE; i++)
    {
        if ((*Nodes)[i].Data == NULL)
        {
            (*Nodes)[i] = *Node;
            return;
        }
    }
}

void SensitiveWordHelper::Print()
{
    Node* CurNode = &Root;

    PrintChildren(CurNode->Children);
}

void SensitiveWordHelper::PrintChildren(Node** Nodes)
{
    for (int i = 0; i < MAX_NODE; i++)
    {
        std::cout << " \n ";

        if ((*Nodes)[i].Data == NULL)
        {
            break;
        }

        std::cout << " - " << (*Nodes)[i].Data;
        PrintChildren((*Nodes)[i].Children);
    }
}