#pragma once
#include "TabFileMacro.h"

TAB_FILE_DATA(FSceneTabData)
{
    int32 SceneId;
    FString Descriptors;

    FSceneTabData()
        : SceneId(-1)
    {}

    TAB_FILE_DATA_SINGLE_KEY(int32, SceneId);

    // 高级要求，load整张表只调用一次
    virtual void RegisterParams() override
    {
        TAB_FILE_DATA_REGISTER(SceneId, "SceneId");
        TAB_FILE_DATA_REGISTER(Descriptors, "Descriptors");
    }
};

TAB_FILE_WITH_PATH(FSceneTabFile, FSceneTabData, "Content/Common/Scene/scene.tab")