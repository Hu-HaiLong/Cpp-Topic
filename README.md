习题编号	系列	难易度	题目	用例	备注

1	学习资料		https://changkun.de/modern-cpp/（用例https://github.com/changkun/modern-cpp-tutorial）		

2	学习资料		程序员的自我修养：链接、装载与库.pdf 		

3	数据结构	简单	实现string	"实现string以下api，内部不能使用string，只能用数组存字符

auto str1 = CustomString(""test1"");
auto str2 = CustomString(""test2, test3"");
str1 = ""test3"";
int len = str1.len();
CustomString sub1 = str1.sub(1,2);
str1.append(""append"");
bool equal = str1 == str2;
int index = str1.find(""es"");
CustomString* ret = str2.split("","");"	
4	数据结构	简单	实现array（可变数组）以及双向链表	"实现数组以及一下api
auto arr = new CustomArray();
arr.reserve(4);
arr.push(1);
arr.push(2);
arr.insert(1, 3);
arr.remove(1);
arr.pop();
arr.clear();
int index = arr.findIndex(1);

实现双向链表以下api
auto list1 = new CustomList();
auto node1 = list1.push(1);
auto node2 = list1.push(2);
auto node3 = list1.insert(node2, 3);
auto ret = list1.find(2);
list1.remove(node2);
list1.popAll();

做完上面的再改成带模板的"	
5	数据结构	简单	ring buffer，要支持buffer size不够时自动扩展大小，以及空余过大时收缩大小	"int initSize = 256;
const char* data = ""123456789"";
auto buffer = new RingBuffer(initSize);
srand(time(NULL));
int length = strlen(data )+1;
for(int ii=0; ii<1000; ii++)
{
    int pushCount = rand()%10;

    for(int jj=0; jj<pushCount; jj++) buffer.push(data, length );    // 要支持自动扩容

    char out[128];
    int ppoCount = rand()%10;
    for(int jj=0; jj<popCount; jj++) buffer.pop(&out[0], sizeof(out));   // 要支持自动缩容
}"	
6	数据结构	简单	通过stack push pop，call的方式(类似lua)调用类的成员函数，要支持返回值	"class Test
{
public:
    int p1 = 1;
    bool p2 = false;
    std::string p3 = ""p33333333"";

    void testWithStack(StackInfo& stack)
    {
        int v1 = 0;
        std::string v2;
        Test v3;
        Test* v4 = nullptr;

        stack.pop<Test*>(v4);
        stack.pop(v3);
        stack.pop(v2);
        stack.pop(v1);

        std::cout << ""testWithStack "" 
            << v1 << "" "" 
            << v2 << "" "" 
            << v3.p3 << "" "" 
            << v4->p3 << std::endl;

        stack.push(1);
    }
};

void test2()
{
    Test t1;

    StackInfo stack;  // 实现此类，就俩模板函数，push和pop
    stack.push(111);
    stack.push<std::string>(""asdfasdfasdf"");

    Test t2;
    t2.p3 = ""t2teststring"";
    stack.push(t2);
    stack.push(&t1);

    t1.testWithStack(stack);

    int ret = 0;
    stack.pop(ret);
    std::cout << ""testWithStack ret "" << ret << std::endl;
}
不允许用any"	

7	数据结构	简单	实现简单八叉树	https://github.com/brandonpelfrey/SimpleOctree/blob/master/main.cpp 	建议根据用例自己写，别上来就看实现

8	数据结构	简单	实现跳表	https://leetcode-cn.com/problems/design-skiplist/	建议根据用例自己写，别上来就看实现

9	C++语法练习	简单	实现对JSON文件的序列化及反序列化	https://github.com/MJPA/SimpleJSON/blob/master/src/demo/example.cpp	建议根据用例自己写，别上来就看实现

10	C++语法练习	简单	实现命令行参数解析以及分发action（类似ts的commander）	https://github.com/geo-data/commander-cpp/blob/master/test/test.cpp	建议根据用例自己写，别上来就看实现

11	C++语法练习	简单	tcp socket练习：写两个exe，一个server，一个client，client和server互相发送消息，接收并打印出来，要求多线程接收发送或者异步接收发送，不能主线程同步接		
12	C++语法练习	简单	基于格子的位置管理：将地图划分为指定大小格子，一个格子中会有多个人物位置信息，根据选定人物的位置查找周围格子（九宫格）的所有位置信息（进阶要求：格子数量超大的时候内存使用要少）	"auto m = new PosManager();

int blockSize = 100;
int mapSize = 10000;
m.init(blockSize, mapSize);

int poseId1 = 1;
int poseId2 = 2;
m.add(poseId1, 100, 200);  // 100是横坐标，200纵坐标
m.add(poseId2, 200, 300);
int[] poseIds = m.find(poseId1);    // 找到poseid1脚下以及周围8格的所有id
m.update(poseId1, 300, 400);
m.remove(poseId2);"	

13	C++语法练习	简单	压缩数据表：读取数据表，将所有表数据存到一个连续buffer中（内存使用尽量少），同时提供查询接口（通过key查找整行数据）	"auto tab = new TabFile();
tab.load(""xxxFile.txt"");
autot lineData = tab.find(""xxxkey"");  // 每行的数据结构可以自己定一个固定的数据结构"	

14	C++语法练习	简单	敏感词匹配（进阶要求：内存使用尽量少）	"auto m = new SensitiveWordHelper();

m.load(""xxxxFile.txt"");
bool ret = m.check(""xxx"");

不许用任何std库的东西（比如std::vector，std::string）"	

15	了解内存	简单	运行时进行“绑定”，能够通过名字可以查找成员变量，还能调用成员函数或者调用静态函数等，实现简单的反射系统	"class Test

{
public:
    int p1 = 1;
    bool p2 = false;
    std::string p3 = ""p33333333"";

    void test1(void* params) 
    {
        std::cout << ""test1"" << std::endl;
    }
    static void test2(void* params)
    {
        std::cout << ""test2"" << std::endl;
    }
};

void test()
{
    Binder<Test> binder;
    binder.bindVar(""p1"", offsetof(Test, p1));
    binder.bindFunc(""test1"", &Test::test1);
    binder.bindStaticFunc(""test2"", Test::test2);

    Test t1;

    int* p1 = binder.findVar<int>(&t1, ""p1"");
    std::cout << *p1 << std::endl;    
    
    binder.callFunc(&t1, ""test1"", nullptr);
    binder.callStaticFunc(""test2"", nullptr);
}


实现Binder"	
16	虚幻练习	简单	熟悉虚幻commandlet，EditorUtility	
"1. 选中动作资源，通过EditorUtility菜单命令导出选中动画的rootmotion位移，按照指定帧率导出成js
2. 通过虚幻commanlet把指定路径下的动画的rootmotion位移按照指定帧率导出成js
导出格式：
[
    {
        path: ""xxx/xxx.uasset"",
        animName: ""xxxx"",
        data: [
            {
                time: 0.033,
                x: 0.0,
                y: 0.0,
                z: 0.0,
                yaw: 0.0
            },
            {
                time: 0.066,
                x: 0.0,
                y: 0.0,
                z: 0.0,
                yaw: 0.0
            }
        ]
    }
]"	

17	虚幻练习	中等	在虚幻内实现海盗读表（需要了解宏以及模板特化）	"TAB_FILE_DATA(FSceneTabData)

{
    int SceneId;
    FString Descriptors;
    // FCustomType CustomValue; // 高级要求，可以支持自定义类型解析

    FSceneTabData()
        : SceneId(-1)
    {}

    TAB_FILE_DATA_SINGLE_KEY(int, SceneId);
    
    // 高级要求，load整张表只调用一次
    virtual void RegisterParams() override
    {
        TAB_FILE_DATA_REGISTER(SceneId, ""id"");
        TAB_FILE_DATA_REGISTER(Descriptors, ""descriptors"");
        // TAB_FILE_DATA_REGISTER(CustomValue, ""custom"");
    } 
};


TAB_FILE_WITH_PATH(FSceneTabFile, FSceneTabData, ""common/scene/scene.tab"")

实现宏 TAB_FILE_DATA、TAB_FILE_DATA_SINGLE_KEY以及TAB_FILE_WITH_PATH

调用TAB_FILE_WITH_PATH后会创建FSceneTabFile并读取 ""common/scene/scene.tab""里的数据，然后根据每行数据生成FSceneTabData实例，最后按照TAB_FILE_DATA_SINGLE_KEY定义的key，存储到FSceneTabFile类中的map里
不允许用rtti、typeid以及UPROPERTY
左侧提到了模板特化，这个不用也能做到，这里要求是因为虚幻里有很多类似的用法，比如NetDeltaSerialize，趁着这题可以练练手"	

18	虚幻练习	困难	阅读虚幻的反射系统，输出笔记		

19	了解内存	简单	了解动态链接库（dll）以及静态链接库的区别，通过名字查找动态链接库中的函数（会使用系统api以及了解原理就行）		

20	了解内存	简单	运行时打印成员变量以及函数地址，打印静态变量以及函数地址（包括虚函数地址）		

21	了解内存	简单	调用系统api打印函数symbol信息，能够打印堆栈信息		

22	内存分配	简单	实现new和delete		

23	内存分配	简单	实现可在栈上分配内存的array，实现后阅读虚幻FArray关于栈上分配内存部分（提示InAllocatorType），并改造成即可支持栈上分配又可支持堆上分配内存的array	FStackArray<ElementType, ElementCount> TestArray;	

24	内存分配	中等	通过实现自定义malloc和free来统计内存，并能够查找所有内存，只能自己内建数据结构（注意要处理自己内部分配的内存）	

25	内存分配	中等	通过实现自定义malloc和free来监视内存		

26	内存分配	中等	阅读虚幻的Malloc，实现可自定义扩展的内存分配器		

27	GC	简单	学习网上的gc机制与策略，输出学习笔记		

28	GC	困难	通过显示调用“ref”与“unref”，书写简单的自动gc机制（至少两种gc策略）	

29	GC	困难	改造“了解内存”系列中的反射系统，去掉上面题中的ref和unref，使用自定义new，做到自动gc	

30	GC	困难	阅读虚幻的gc系统，输出笔记		

31	内存池	简单	实现智能指针		

32	内存池	简单	object池，要使用placement new	

33	内存池	困难	简单内存池，要包含页管理等策略，要处理内部管理时的内存分配问题		

34	调试内存	简单	学会vs的数据断点功能		

35	调试内存	中等	自己书写工具，当内存写越界时崩溃并输出堆栈信息，要能检测出修改静态变量	

36	多线程	简单	无锁队列		
