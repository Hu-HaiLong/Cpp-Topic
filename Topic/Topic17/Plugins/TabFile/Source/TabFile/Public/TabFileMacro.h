#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Misc/FileHelper.h"

#define TAB_FILE_DATA(StructName) struct StructName : FTabData

#define TAB_FILE_DATA_SINGLE_KEY(T, IDKey) typedef T TYPE; TYPE GetKey() {return IDKey;}; 

#define TAB_FILE_DATA_REGISTER(Data, Name) Register(Data, Name)

#define TAB_FILE_WITH_PATH(TabFileStruct, TabDataStruct, TabPath)\
struct TabFileStruct : public TabFileBase<TabDataStruct, TabFileStruct>\
{\
    static inline FString TmpTabPath = TabPath;\
\
    static FString GetFilePath()\
    {\
        return TmpTabPath;\
    }\
};\


struct FTabData
{
public:
    virtual ~FTabData() {};

    void Register(int32& Intance, const FString& Name);
    void Register(FString& Intance, const FString& Name);
    void AddData(TArray<FString> TabDatas, int32 Line);

    virtual void RegisterParams() = 0;

private:
    TMap<FString, int32*> Int_Datas;
    TMap<FString, FString*> Str_Datas;
};

void FTabData::AddData(TArray<FString> TabDatas, int32 Line)
{
    UE_LOG(LogTemp, Warning, TEXT("[AddData] "));

    TArray<FString> Names;
    TabDatas[0].ParseIntoArray(Names, TEXT("\t"));

    TArray<FString> Values;
    TabDatas[Line].ParseIntoArray(Values, TEXT("\t"));

    int32 Index = 0;

    for (FString Name : Names)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Value] %s"), *Name);

        if (int32** TempInt = Int_Datas.Find(Name))
        {
            **TempInt = FCString::Atoi(*Values[Index]);
            UE_LOG(LogTemp, Warning, TEXT(" -- Value  %d"), **TempInt);
        }
        else if (FString** TempStr = Str_Datas.Find(Name))
        {
            **TempStr = Values[Index];
            UE_LOG(LogTemp, Warning, TEXT(" -- Value  %s   --  %s"), ***TempStr, **Str_Datas[Name]);
        }

        ++Index;
    }
};

void FTabData::Register(int32& Intance, const FString& Name)
{
    Int_Datas.Add(Name, &Intance);
};

void FTabData::Register(FString& Intance, const FString& Name)
{
    Str_Datas.Add(Name, &Intance);
};

template <typename TabDataStruct, typename TabFileStruct>
struct TabFileBase
{
public:
    TabFileBase();
    virtual ~TabFileBase() {};

    TMap<typename TabDataStruct::TYPE, TabDataStruct> GetDatas();

private:
    TMap<typename TabDataStruct::TYPE, TabDataStruct> Datas;
};

template <typename TabDataStruct, typename TabFileStruct>
TabFileBase<TabDataStruct, TabFileStruct>::TabFileBase()
{
    FString TabFullPath = FPaths::ProjectDir() + TabFileStruct::GetFilePath();
    if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*TabFullPath))
    {
        return;
    }
    TArray<FString> TabDatas;
    FFileHelper::LoadFileToStringArray(TabDatas, *TabFullPath);
    int32 Line = 0;
    for (FString LineContent : TabDatas)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Line %d] %s"), Line, *LineContent);
        if (Line == 0)
        {
            Line++;
            continue;
        }
        TabDataStruct* TabDataIns = new TabDataStruct();
        TabDataIns->RegisterParams();
        TabDataIns->AddData(TabDatas, Line);
        Datas.Add(TabDataIns->GetKey(), *TabDataIns);
        Line++;
    }
}

template <typename TabDataStruct, typename TabFileStruct>
TMap<typename TabDataStruct::TYPE, TabDataStruct> TabFileBase<TabDataStruct, TabFileStruct>::GetDatas()
{
    return Datas;
};