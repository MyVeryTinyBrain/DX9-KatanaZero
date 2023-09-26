#include "stdafx.h"
#include "Stage2.h"
#include "Stage3.h"
#include "SoundMgr.h"

Scene* Stage2::NextScene()
{
    return new Stage3;
}

wstring Stage2::StagePath()
{
    return L"../Stage/stage_2.txt";
}

Scene* Stage2::Copy()
{
    return new Stage2;
}

void Stage2::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_chinatown.ogg");
}
