
#include "Binder.h"

class Test
{
public:
    int P1 = 11;
    int P4 = 22;
    bool P2 = false;
    std::string P3 = "p33333333";

    void Test1(void* Params)
    {
        std::cout << "  Test1  " << std::endl;
    }
    static void test2(void* Params)
    {
        std::cout << "  Test2  " << *(int*)Params << std::endl;
    }
};

void test()
{
    Binder<Test> Binder;
    Binder.BindVar("P4", offsetof(Test, P4));
    Binder.BindFunc("Test1", &Test::Test1);
    Binder.BindStaticFunc("Test2", Test::test2);

    Test T1;

    int* P1 = Binder.FindVar<int>(&T1, "P4");
    std::cout << *P1 << std::endl;

    Binder.CallFunc(&T1, "Test1", nullptr);
    
    int TestA = 666;
    Binder.CallStaticFunc("Test2", &TestA);
}

int main()
{
    test();
}