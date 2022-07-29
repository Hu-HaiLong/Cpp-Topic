#include "SensitiveWordHelper.h" 

int main()
{
    auto MyHelper = new SensitiveWordHelper();
    MyHelper->Load("xxxFile.txt");
    bool bCheck = MyHelper->Check("fucd查ca3d2n");

    printf(bCheck ? "true\n" : "false\n");
    MyHelper->Print();

    return 0;
}