#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Misc/FileHelper.h"

struct FTabData
{
public:
    virtual ~FTabData() {};

    void Register(int32& Intance, const FString& Name);
    void Register(FString& Intance, const FString& Name);
    void AddData(TArray<FString> TabDatas, int32 Line);

    void Print();
    
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
        else if(FString** TempStr = Str_Datas.Find(Name))
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

void FTabData::Print()
{
    UE_LOG(LogTemp, Warning, TEXT("[Print]"));

    for (TPair<FString, int32*>& Data1 : Int_Datas)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Int_Datas] ：%s -- %d"), *Data1.Key, *Data1.Value);
    }

    for (TPair<FString, FString*>& Data2 : Str_Datas)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Str_Datas] ：%s -- %s"), *Data2.Key, **Data2.Value);
    }
};

//
#define TAB_FILE_DATA(StructName) struct StructName : FTabData
#define TAB_FILE_DATA_SINGLE_KEY(T, IDKey) typedef T TYPE; TYPE GetKey() {return IDKey;}; 
#define TAB_FILE_DATA_REGISTER(Data, Name) Register(Data, Name)
//TAB_FILE_DATA
TAB_FILE_DATA(FSceneTabData)
{
    int32 SceneId;
    FString Descriptors;
    // FCustomType CustomValue; // 高级要求，可以支持自定义类型解析

    FSceneTabData()
        : SceneId(-1)
    {}

    TAB_FILE_DATA_SINGLE_KEY(int32, SceneId);

    // 高级要求，load整张表只调用一次
    virtual void RegisterParams() override
    {
        TAB_FILE_DATA_REGISTER(SceneId, "SceneId");
        TAB_FILE_DATA_REGISTER(Descriptors, "Descriptors");
        // TAB_FILE_DATA_REGISTER(CustomValue, "Custom");
    }
};

#define TAB_FILE_WITH_PATH(TabFileStruct, TabDataStruct, TabPath)\
struct TabFileStruct\
{\
    TabFileStruct()\
    {\
        FString TabFullPath = FPaths::ProjectDir() + TabPath;\
        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*TabFullPath))\
        {\
            TArray<FString> TabDatas;\
            FFileHelper::LoadFileToStringArray(TabDatas, *TabFullPath);\
            int32 Line = 0;\
            for (FString LineContent : TabDatas)\
            {\
                UE_LOG(LogTemp, Warning, TEXT("[Line %d] %s"), Line, *LineContent);\
                if (Line == 0)\
                {\
                    Line++;\
                    continue;\
                }\
                TabDataStruct* TabDataIns = new TabDataStruct();\
                TabDataIns->RegisterParams();\
                TabDataIns->AddData(TabDatas, Line);\
                Datas.Add(TabDataIns->GetKey(), *TabDataIns);\
                Line++;\
            }\
        }\
        UE_LOG(LogTemp, Warning, TEXT("[TabFileStruct TEST Print]"));\
        for (auto& Data : Datas)\
        {\
            Data.Value.Print();\
        }\
    }\
private:\
    TMap<TabDataStruct::TYPE, TabDataStruct> Datas;\
};\
TabFileStruct();\

//TAB_FILE_WITH_PATH(FSceneTabFile, FSceneTabData, "Content/Common/Scene/scene.tab")