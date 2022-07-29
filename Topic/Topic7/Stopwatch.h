// Copy from use case

#include <windows.h>
double WatchTime()
{
    unsigned long long Ticks = 0;
    unsigned long long Ticks_Per_Sec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&Ticks_Per_Sec);
    QueryPerformanceCounter((LARGE_INTEGER*)&Ticks);
    return ((float)Ticks) / (float)Ticks_Per_Sec;
}