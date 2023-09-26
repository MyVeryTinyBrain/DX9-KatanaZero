#include "stdafx.h"
#include "EnemyGangsterAnimator.h"
#include "AnimationManager.h"

void EnemyGangsterAnimator::InitAnimations()
{
	m_idle = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/idle.txt");
	m_walk = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/walk.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/run.txt");
	m_turn = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/turn.txt");
	m_usedoor = m_turn;
	m_hurtfly = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/hurtfly.txt");
	m_hurtground = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/hurtground.txt");
	m_melee = AnimationManager::GetAnimation(L"../Animation/enemy/gangster/melee.txt");

	wstring localDirectory = L"../Texture/enemy/used/gangster/aim/";
	wstring fileName = L"spr_gangsteraim_";
	for (int i = 0; i < 19; ++i)
	{
		WCHAR buf[32]{};
		_itow_s(i, buf, 10);
		m_aim[i] = new Sprite(localDirectory + fileName + buf + L".png");
	}

	m_aimAdjustY = 0;
}
