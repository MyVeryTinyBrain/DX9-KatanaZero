#include "stdafx.h"
#include "SojuBottle.h"
#include "SoundMgr.h"

void SojuBottle::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
    CSoundMgr::Get_Instance()->PlaySound(L"sound_glassbreak1.wav", CSoundMgr::CHANNELID::THROW_OBJECT);
    MakePiecesWhenTrigger(L"../Texture/object/used/throw/spr_beer_bottle_3_0_broke.png");
}

wstring SojuBottle::SpriteKey()
{
    return L"../Texture/object/used/throw/spr_beer_bottle_3_0.png";
}
