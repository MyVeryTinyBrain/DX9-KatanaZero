#include "stdafx.h"
#include "NextSceneTriggerCallback.h"
#include "GameTag.h"
#include "StageScene.h"
#include "Enemy.h"
#include "SceneChangeEffect.h"
#include "Recorder.h"
#include "PlayerController.h"

NextSceneTriggerCallback::NextSceneTriggerCallback()
{
	m_used = false;
	m_useRecordPlayback = true;
}

void NextSceneTriggerCallback::OnUpdate()
{
	if (Enemy::g_enemies.size() > 0) return;
	if (m_used) return;

	StageScene* stageScene = dynamic_cast<StageScene*>(SceneManager::GetCurrentScene());
	if (!stageScene) return;

	const Vec2& playerPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
	const Vec2& thisBottomLeftPos = m_trigger->GetCollider()->GetBottomLeft();
	const Vec2& thisBottomRightPos = m_trigger->GetCollider()->GetBottomLeft();
	Vec2 bottom = (thisBottomLeftPos + thisBottomRightPos) * 0.5f;
	if (Vec2::Distance(playerPos, bottom) <= 75)
	{
		stageScene->ChangeToNextScene();
		m_used = true;
	}
}

void NextSceneTriggerCallback::OnTrigger(Collider* collider)
{
	if (collider->GetBody()->GetGameObject()->GetTag() == TAG_PLAYER)
	{
		StageScene* stageScene = dynamic_cast<StageScene*>(SceneManager::GetCurrentScene());
		if (!stageScene) return;
		if (Enemy::g_enemies.size() > 0) return;
		if (m_used) return;

		m_used = true;

		stageScene->ChangeToNextScene();
	}
}
