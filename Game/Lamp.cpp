#include "stdafx.h"
#include "Lamp.h"
#include "SoundMgr.h"

void Lamp::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
	CSoundMgr::Get_Instance()->PlaySound(L"sound_object_floorbreak_stone_01.wav", CSoundMgr::CHANNELID::THROW_OBJECT);
	MakePiecesWhenTrigger(L"../Texture/object/used/throw/spr_lamp_0_broke.png");
}

wstring Lamp::SpriteKey()
{
	return L"../Texture/object/used/throw/spr_lamp_0.png";
}
