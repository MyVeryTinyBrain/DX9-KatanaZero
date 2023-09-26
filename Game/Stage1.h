#pragma once

#include "StageScene.h"

class Stage1 : public StageScene
{
    virtual Scene* NextScene() override;
    virtual wstring StagePath() override;
    virtual Scene* Copy() override;
    virtual void OnLoad(Scene* prevScene) override;
};

