#pragma once

#include "CollidedGround.h"

class PlatformerBody;

class Boss : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);

	PROTECTED virtual void OnCollisonEnter(const CollisionResult& collision) {}
	PROTECTED virtual void OnCollisonExit(Collider* collider) {}
	PROTECTED virtual void OnCollisonStay(const CollisionResult& collision) {}
	PROTECTED virtual void OnTriggerEnter(Collider* collider) {};
	PROTECTED virtual void OnTriggerExit(Collider* collider) {};
	PROTECTED virtual void OnTriggerStay(Collider* collider) {};
	PROTECTED virtual void OnGrounded(const Vec2& point, const Vec2& normal) {}

	PUBLIC bool TakeDamage(Vec2 velocity, Component* damageObject, bool canBlock = false);
	PUBLIC virtual bool CanTakeDamage() const = 0;
	PROTECTED virtual bool Damage(Vec2 velocity, Component* damageObject, bool canBlock = false) = 0;

	PUBLIC void SetDirection(float hd);
	PUBLIC float GetDirection() const;

	PUBLIC bool GroundCheck(RaycastResult* pResult, float length = 1.0f) const;
	PUBLIC bool GroundCheck(float length = 1.0f) const;
	PUBLIC void DetachFromGround();
	PUBLIC bool HasGround() const;

	PUBLIC void SetHeight(float height);
	PUBLIC void SetWidth(float width);
	PUBLIC float GetHeight() const;
	PUBLIC float GetWidth() const;
	PUBLIC Vec2 GetBottom() const;

	PUBLIC void SetSlowModeColorChange(bool enable);

	PROTECTED Body* m_body;
	PROTECTED BoxCollider* m_boxCollider;
	PROTECTED CircleCollider* m_circleCollider;
	PRIVATE bool m_ground;
	PROTECTED float m_groundDamping;
	PRIVATE std::vector<CollidedGround> m_grounds;

	PROTECTED GameObject* m_rendererParentObject;
	PROTECTED SpriteRenderer* m_renderer;

	PRIVATE bool m_slowModeColorChange;

	PRIVATE float m_slowTimeTrailCounter;
};

