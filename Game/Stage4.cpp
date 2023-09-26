#include "stdafx.h"
#include "Stage4.h"
#include "StageHeadhunter.h"
#include "SoundMgr.h"

Scene* Stage4::NextScene()
{
    return new StageHeadhunter;
}

wstring Stage4::StagePath()
{
    return L"../Stage/stage_4.txt";
}

Scene* Stage4::Copy()
{
    return new Stage4;
}

void Stage4::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_chinatown.ogg");
}
