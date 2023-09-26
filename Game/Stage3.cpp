#include "stdafx.h"
#include "Stage3.h"
#include "Stage4.h"
#include "SoundMgr.h"

Scene* Stage3::NextScene()
{
    return new Stage4;
}

wstring Stage3::StagePath()
{
    return L"../Stage/stage_3.txt";
}

Scene* Stage3::Copy()
{
    return new Stage3;
}

void Stage3::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_chinatown.ogg");
}
