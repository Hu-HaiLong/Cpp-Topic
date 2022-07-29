#pragma once
#include <iostream>
#include <map>

using namespace std;

template <typename T>
class Binder
{
    typedef void (T::* Func)(void*);
    typedef void (*StaticFunc)(void*);

public:
    void BindVar(const char* Name, int Offset)
    {
        Values[Name] = Offset;
    };

    void BindFunc(const char* Name, Func InFunc)
    {
        Funcs[Name] = InFunc;
    };

    void BindStaticFunc(const char* Name, StaticFunc InFunc)
    {
        StaticFuncs[Name] = InFunc;
    };

    template <typename P>
    P* FindVar(T* t, const char* Name)
    {
        map<const char*, int>::iterator iter;

        for (iter = Values.begin(); iter != Values.end(); iter++)
        {
            if (iter->first == Name)
            {
                return (P*)((size_t)(t)+iter->second);
            }
        }

        return nullptr;
    };

    void CallFunc(T* InT, const char* Name, void* Param)
    {
        typename map<const char*, Func>::iterator iter;

        for (iter = Funcs.begin(); iter != Funcs.end(); iter++)
        {
            if (iter->first != Name)
            {
                continue;
            }

            Func TmpFunc = iter->second;

            if (!TmpFunc)
            {
                return;
            }

            (*InT.*TmpFunc)(Param);
        }
    };

    void CallStaticFunc(const char* Name, void* Param)
    {
        typename map<const char*, StaticFunc>::iterator iter;

        for (iter = StaticFuncs.begin(); iter != StaticFuncs.end(); iter++)
        {
            if (iter->first != Name)
            {
                continue;
            }

            StaticFunc TmpFunc = iter->second;

            if (!TmpFunc)
            {
                return;
            }

            (*TmpFunc)(Param);
        }
    };

private:
    map<const char*, int> Values;
    map<const char*, Func> Funcs;
    map<const char*, StaticFunc> StaticFuncs;
};
