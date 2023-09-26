#pragma once

#include "ThrowObjectMode.h"

class DownArrow;

class ThrowObject : public Component
{
	PUBLIC enum class BreakType { STAGE, ENEMY, BULLET };

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnTriggerEnter(Collider* collider);

	PROTECTED virtual void OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType) = 0;
	PROTECTED virtual void OnPutIn() {}
	PROTECTED virtual wstring SpriteKey() = 0;

	PUBLIC const wstring& GetSpriteKey() const;
	PUBLIC bool CanCrashWithBullet() const;
	PUBLIC void Crash(const Vec2& velocity);

	PUBLIC void Throw(const Vec2& position, const Vec2& velocity);

	PROTECTED void MakePiecesWhenTrigger(const wstring& pieceSpriteKey);

	PRIVATE void PutIn();
	PRIVATE void Break(BreakType breakType);

	PROTECTED void DestroyDustEffect();

	PROTECTED SpriteRenderer* m_renderer;
	PROTECTED Sprite* m_sprite;
	PRIVATE ThrowObjectMode m_mode;
	
	PROTECTED Body* m_body;
	PROTECTED CircleCollider* m_trigger;

	PRIVATE bool m_destroyWhenNextFrame;
	PROTECTED bool m_damageToEnemy;

	// 적과 충돌했을 때
	// 방패병만 이 오브젝트를 막을 수 있습니다.
	PROTECTED bool m_canOnlyBlockWithShield;
	PROTECTED bool m_canCrashWithBullet;
	PROTECTED bool m_canReflectByBullet;
	PROTECTED bool m_useBreakEffect;

	PRIVATE float m_throwingElapsed;

	PRIVATE DownArrow* m_arrow;
};

