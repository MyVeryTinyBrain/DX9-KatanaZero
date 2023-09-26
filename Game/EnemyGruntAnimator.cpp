#include "stdafx.h"
#include "EnemyGruntAnimator.h"
#include "AnimationManager.h"

void EnemyGruntAnimator::InitAnimations()
{
	m_idle = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/idle.txt");
	m_walk = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/walk.txt");
	m_run = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/run.txt");
	m_turn = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/turn.txt");
	m_usedoor = m_turn;
	m_hurtfly = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/hurtfly.txt");
	m_hurtground = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/hurtground.txt");
	m_melee = AnimationManager::GetAnimation(L"../Animation/enemy/grunt/melee.txt");
}
