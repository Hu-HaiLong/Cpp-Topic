// Fill out your copyright notice in the Description page of Project Settings.


#include "RMBlueprintFunctionLibrary.h"
#include "JsonObjectConverter.h"

void URMBlueprintFunctionLibrary::ExportRootMotion(TArray<UObject*> Objs)
{
    UE_LOG(LogCommandletExample, Log, TEXT("ExportRootMotion begin to execute ."));

    FExportStruct Tmp;

    for (auto Obj : Objs)
    {
        UAnimSequence* AnimS = Cast<UAnimSequence>(Obj);

        if (!AnimS)
        {
            continue;
        }

        FAnimStruct TmpAnim;
        TmpAnim.Path = AnimS->GetFullName();
        TmpAnim.AnimName = AnimS->GetName();

        FRawAnimSequenceTrack Track = AnimS->GetDataModel()->GetBoneTrackByName("Root").InternalTrackData;
        TArray<FVector3f> PosKeys = Track.PosKeys;
        TArray<FQuat4f> RotKeys = Track.RotKeys;
        int32 SampleKeys = AnimS->GetNumberOfSampledKeys();

        for (size_t i = 0; i < SampleKeys; i++)
        {
            UE_LOG(LogCommandletExample, Log, TEXT(" --  execute .  i : %d"), i);
            UE_LOG(LogCommandletExample, Log, TEXT(" --  X : %d  Y: %f  Z :%f Yaw :%f"), PosKeys[i].X, PosKeys[i].Y, PosKeys[i].Z, RotKeys[i].Rotator().Yaw);

            FAnimTrackStruct TmpAnimTrack;
            TmpAnimTrack.Time = AnimS->GetTimeAtFrame(i);
            TmpAnimTrack.X = PosKeys[i].X;
            TmpAnimTrack.Y = PosKeys[i].Y;
            TmpAnimTrack.Z = PosKeys[i].Z;
            TmpAnimTrack.Yaw = RotKeys[i].Rotator().Yaw;

            TmpAnim.Data.Add(TmpAnimTrack);
        }

        Tmp.Data.Add(TmpAnim);
    }

    FString Json;

    if (FJsonObjectConverter::UStructToJsonObjectString(Tmp, Json))
    {
        FString FileLocation = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + TEXT("RootMotion"));
        FFileHelper::SaveStringToFile(Json, *FileLocation);
    }

}
