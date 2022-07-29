
#include "Skiplist.h"

int main()
{
    Skiplist* Obj = new Skiplist();

    Obj->Add(19);
    Obj->Add(7);
    Obj->Add(3);
    Obj->Add(17);
    Obj->Add(11);
    Obj->Add(5);

    Obj->Print();

    bool Param1 = Obj->Search(2);
    bool Param2 = Obj->Erase(5);
    bool Param3 = Obj->Search(5);
    bool Param4 = Obj->Erase(3);
    Obj->Add(4);

    Obj->Print();

    return 0;
}