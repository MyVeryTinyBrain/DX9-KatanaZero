#include "stdafx.h"
#include "Stage1.h"
#include "Stage2.h"
#include "SoundMgr.h"

Scene* Stage1::NextScene()
{
    return new Stage2;
}

wstring Stage1::StagePath()
{
    return L"../Stage/stage_1.txt";
}

Scene* Stage1::Copy()
{
    return new Stage1;
}

void Stage1::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_chinatown.ogg");
}
