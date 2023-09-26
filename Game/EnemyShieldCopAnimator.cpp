#include "stdafx.h"
#include "EnemyShieldCopAnimator.h"
#include "AnimationManager.h"

void EnemyShieldCopAnimator::InitAnimations()
{
	m_idle = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/idle.txt");
	m_walk = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/walk.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/run.txt");
	m_turn = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/turn.txt");
	m_usedoor = m_turn;
	m_hurtfly = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/hurtfly.txt");
	m_hurtground = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/hurtground.txt");
	m_groggy = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/knockback.txt");
	m_melee = AnimationManager::GetAnimation(L"../Animation/enemy/shieldcop/melee.txt");

	wstring localDirectory = L"../Texture/enemy/used/shieldcop/aim/";
	wstring fileName = L"spr_shieldcop_aim_";
	for (int i = 0; i < 19; ++i)
	{
		WCHAR buf[32]{};
		_itow_s(i, buf, 10);
		m_aim[i] = new Sprite(localDirectory + fileName + buf + L".png");
	}

	m_aimAdjustY = -1;
}
