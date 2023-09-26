#include "stdafx.h"
#include "EnemyPompAnimator.h"
#include "AnimationManager.h"

void EnemyPompAnimator::InitAnimations()
{
	m_idle = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/idle.txt");
	m_walk = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/walk.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/run.txt");
	m_turn = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/turn.txt");
	m_usedoor = m_turn;
	m_hurtfly = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/hurtfly.txt");
	m_hurtground = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/hurtground.txt");
	m_groggy = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/knockback.txt");
	m_melee = AnimationManager::GetAnimation(L"../Animation/enemy/pomp/melee.txt");
}
