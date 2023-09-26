#include "stdafx.h"
#include "EnemyDeadBody.h"
#include "GameRenderOrder.h"
#include "Enemy.h"

void EnemyDeadBody::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
}

void EnemyDeadBody::OnDestroy()
{
	Sprite* sprite = m_renderer->GetSprite();
	SAFE_DELETE(sprite);
}

void EnemyDeadBody::SetDeadBody(Enemy* enemy)
{
	auto renderer = enemy->GetRenderer();
	if (!renderer->GetSprite()) return;
	GetTransform()->SetTransform(
		renderer->GetTransform()->GetPosition(),
		renderer->GetTransform()->GetAngle(),
		renderer->GetTransform()->GetScale());
	m_renderer->SetSprite(new Sprite(*renderer->GetSprite()));
	m_renderer->SetRenderOrder(RENDER_ORDER_ENEMY + enemy->GetEnemyID() * 5);
}
