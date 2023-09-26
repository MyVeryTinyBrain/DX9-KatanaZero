#include "stdafx.h"
#include "EnemyShotgunAnimator.h"
#include "AnimationManager.h"

void EnemyShotgunAnimator::InitAnimations()
{
	m_idle = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/idle.txt");
	m_walk = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/walk.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/run.txt");
	m_turn = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/turn.txt");
	m_usedoor = m_turn;
	m_hurtfly = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/hurtfly.txt");
	m_hurtground = AnimationManager::GetAnimation(L"../Animation/enemy/shotgun/hurtground.txt");

	wstring localDirectory = L"../Texture/enemy/used/shotgun/aim/";
	wstring fileName = L"spr_shotgun_aim_";
	for (int i = 0; i < 19; ++i)
	{
		WCHAR buf[32]{};
		_itow_s(i, buf, 10);
		m_aim[i] = new Sprite(localDirectory + fileName + buf + L".png");
	}

	m_aimAdjustY = 0;
}
