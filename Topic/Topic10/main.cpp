#include <stdio.h>
#include "Commander.h"

static void DoVerbose(Command* InCmd)
{
    printf("verbose: enabled\n");
}

static void DoRequired(Command* InCmd)
{
    printf("required: %s\n", InCmd->Arg);
}

static void DoOption(Command* InCmd)
{
    printf("optional: %s\n", InCmd->Arg);
}

int main(int ArgC, const char** ArgV)
{
    Command Cmd;
    Commander::Init(&Cmd, ArgV[0], "0.0.3");
    Commander::Option(&Cmd, "-v", "--verbose", "enable verbose stuff", DoVerbose);
    Commander::Option(&Cmd, "-r", "--required <arg>", "required arg", DoRequired);
    Commander::Option(&Cmd, "-o", "--optional [arg]", "optional arg", DoOption);
    Commander::Parse(&Cmd, ArgC, ArgV);

    for (int i = 0; i < ArgC; ++i) 
    {
        printf("  - '%s'\n", ArgV[i]);
    }

    printf("additional args:\n");

    for (int i = 0; i < Cmd.ArgC; ++i) 
    {
        printf("  - '%s'\n", Cmd.ArgV[i]);
    }

    return 0;
}