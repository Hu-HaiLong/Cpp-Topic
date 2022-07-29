#pragma once

#include <sstream>
#include "stdlib.h"

#ifndef COMMANDER_MAX_ARGS
#define COMMANDER_MAX_ARGS 32
#endif

#ifndef COMMANDER_MAX_OPTIONS
#define COMMANDER_MAX_OPTIONS 32
#endif

struct Command;

typedef void (*CommandCallBack)(Command* InCmd);

typedef struct Optional
{
    char* ArgName;
    char* Long;
    const char* Short;
    const char* LongArg;
    const char* Description;

    bool bOptionalArg = false;
    bool bRequiredArg = false;

    CommandCallBack Callback;
} Optional;

typedef struct Command
{
    const char* Name;
    const char* Version;
    const char* Usage;
    int ArgC;
    char* ArgV[COMMANDER_MAX_ARGS];
    const char* Arg;

    int OptionCount;
    Optional Options[COMMANDER_MAX_OPTIONS];
} Command;

class Commander
{
public:
    Commander() {};

    static void Init(Command* InCmd, const char* InName, const char* InVersion)
    {
        InCmd->Name = InName;
        InCmd->Version = InVersion;
        InCmd->Arg = NULL;
        InCmd->ArgC = 0;
        InCmd->OptionCount = 0;
        InCmd->Usage = "[options] [args]";
        Commander::Option(InCmd, "-ver", "--version", "version balabala ", Version);
        Commander::Option(InCmd, "-h", "--help", "help balabala ", Help);
    }

    static void Option(Command* InCmd, const char* InShort, const char* InLong, const char* InDesc, CommandCallBack InCallBack)
    {
        if (InCmd->OptionCount == COMMANDER_MAX_OPTIONS)
        {
            Free(InCmd);
            return;
        }

        int CurIndex = InCmd->OptionCount++;
        Optional* Opt = &InCmd->Options[CurIndex];
        Opt->Short = InShort;
        Opt->LongArg = InLong;
        Opt->Description = InDesc;
        Opt->Callback = InCallBack;

        Opt->Long = (char*)malloc(strlen(InLong) * sizeof(char) + 1);
        memset(Opt->Long, 0, strlen(InLong) * sizeof(char) + 1);
        Opt->ArgName = (char*)malloc(strlen(InLong) * sizeof(char) + 1);
        memset(Opt->ArgName, 0, strlen(InLong) * sizeof(char) + 1);
        ParseArgName(InLong, Opt->Long, Opt->ArgName);

        if (Opt->ArgName != NULL && Opt->ArgName[0] == '<')
        {
            Opt->bRequiredArg = true;
        }

        if (Opt->ArgName != NULL && Opt->ArgName[0] == '[')
        {
            Opt->bOptionalArg = true;
        }
    }

    static void Parse(Command* InCmd, int InArgC, const char** InArgV)
    {
        ParseImpl(InCmd, InArgC, InArgV);
    }

private:
    static void Version(Command* InCmd)
    {
        printf("%s\n", InCmd->Version);
        Free(InCmd);
        exit(0);
    }

    static void Help(Command* InCmd)
    {
        printf("  Usage: %s %s\n", InCmd->Name, InCmd->Usage);
        printf("  Options:\n");

        for (int i = 0; i < InCmd->OptionCount; ++i)
        {
            Optional* Option = &InCmd->Options[i];
            printf("\t%-7s%-20s%s\n"
                , Option->Short
                , Option->LongArg
                , Option->Description);
        }

        Free(InCmd);
        exit(0);
    }

    // 解析 指令参数格式
    static void ParseArgName(const char* Long, char* LongName, char* ArgName)
    {
        bool bArg = false;
        int ArgNameIndex = 0;
        int LongNameIndex = 0;

        for (size_t i = 0; i < strlen(Long); i++)
        {
            if (Long[i] == '<' || Long[i] == '[' || bArg)
            {
                bArg = true;
                ArgName[ArgNameIndex++] = Long[i];
            }
            else
            {
                if (IsWhitespace(Long[i]))
                {
                    continue;
                }

                memcpy(&LongName[LongNameIndex++], &Long[i], sizeof(char));
            }
        }
    }

    static bool IsWhitespace(char InC)
    {
        if (InC == ' ' || InC == '\t' || InC == '\r' || InC == L'\n')
        {
            return true;
        }

        return false;
    }

    // 释放
    static void Free(Command* InCmd)
    {
        for (int i = 0; i < InCmd->OptionCount; i++)
        {
            Optional* CurOption = &InCmd->Options[i];
            delete CurOption->Long;
            delete CurOption->ArgName;
        }
    }

    // 解析指令实现
    static void ParseImpl(Command* InCmd, int ArgC, const char** ArgV)
    {
        for (int i = 1; i < ArgC; i++)
        {
            const char* CurArg = ArgV[i];

            OptionMatch(InCmd, ArgV, i);

            if (InCmd->ArgC + 1 == COMMANDER_MAX_ARGS)
            {
                // error
                Free(InCmd);
                exit(1);
            }

            InCmd->ArgV[InCmd->ArgC++] = (char*)CurArg;
        }
    }

    static void OptionMatch(Command* InCmd, const char** ArgV, int InIndex)
    {
        const char* CurArg = ArgV[InIndex];

        for (int j = 0; j < InCmd->OptionCount; j++)
        {
            Optional* CurOptional = &InCmd->Options[j];

            // 指令匹配
            if (strcmp(CurArg, CurOptional->Short) != 0 && strcmp(CurArg, CurOptional->Long) != 0)
            {
                continue;
            }

            InCmd->Arg = NULL;

            if (CurOptional->bOptionalArg)
            {
                if (ArgV[InIndex + 1] && ArgV[InIndex + 1][0] != '-')
                {
                    InCmd->Arg = ArgV[++InIndex];
                }
            }

            if (CurOptional->bRequiredArg)
            {
                if (ArgV[InIndex + 1] && ArgV[InIndex + 1][0] != '-')
                {
                    InCmd->Arg = ArgV[++InIndex];
                }
                else
                {
                    Free(InCmd);
                    fprintf(stderr, "%s %s argument required\n", CurOptional->Long, CurOptional->ArgName);
                    exit(1);
                }
            }

            CurOptional->Callback(InCmd);
            continue;
        }
    }
};
