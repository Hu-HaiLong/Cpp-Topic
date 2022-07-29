#include "TabFile.h"

int main()
{
    printf("sad");
    auto Tab = new TabFile();
    Tab->Load("xxxFile.txt");
    auto lineData = Tab->Find("3");  // 每行的数据结构可以自己定一个固定的数据结构

    system("pause");

    return 0;
}
