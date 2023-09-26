#include "stdafx.h"
#include "BearBottle.h"
#include "SoundMgr.h"

void BearBottle::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
    MakePiecesWhenTrigger(L"../Texture/object/used/throw/spr_beer_bottle_2_0_broke.png");

    CSoundMgr::Get_Instance()->PlaySound(L"sound_glassbreak1.wav", CSoundMgr::CHANNELID::THROW_OBJECT);
}

wstring BearBottle::SpriteKey()
{
    return L"../Texture/object/used/throw/spr_beer_bottle_2_0.png";
}
