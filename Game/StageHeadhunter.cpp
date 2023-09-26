#include "stdafx.h"
#include "StageHeadhunter.h"
#include "SoundMgr.h"
#include "MainScene.h"

Scene* StageHeadhunter::NextScene()
{
    return new MainScene;
}

wstring StageHeadhunter::StagePath()
{
    return L"../Stage/stage_boss.txt";
}

Scene* StageHeadhunter::Copy()
{
    return new StageHeadhunter;
}

void StageHeadhunter::OnLoad(Scene* prevScene)
{
    StageScene::OnLoad(prevScene);
    CSoundMgr::Get_Instance()->PlayBGM(L"song_fullconfession.ogg");
}
