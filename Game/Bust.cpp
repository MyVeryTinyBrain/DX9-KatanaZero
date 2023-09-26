#include "stdafx.h"
#include "Bust.h"
#include "SoundMgr.h"

void Bust::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
	MakePiecesWhenTrigger(L"../Texture/object/used/throw/spr_bust_0_broke.png");

	CSoundMgr::Get_Instance()->PlaySound(L"sound_object_floorbreak_stone_01.wav", CSoundMgr::CHANNELID::THROW_OBJECT);
}

wstring Bust::SpriteKey()
{
	return L"../Texture/object/used/throw/spr_bust_0.png";
}
