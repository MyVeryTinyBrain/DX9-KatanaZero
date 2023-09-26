#include "stdafx.h"
#include "Stage0.h"
#include "Stage1.h"
#include "SoundMgr.h"

Scene* Stage0::NextScene()
{
    return new Stage1;
}

wstring Stage0::StagePath()
{
    return L"../Stage/stage_0.txt";
}

Scene* Stage0::Copy()
{
    return new Stage0;
}

void Stage0::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_chinatown.ogg");
}
