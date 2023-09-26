#pragma once

#include "CollidedGround.h"

class Body; 

class PlatformerBody : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(LateFixedUpdate);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE void OnDestroyBody(Object* body);
	PRIVATE void OnDestroyCollider(Object* collider);
	PRIVATE void OnCollisionEnter(const CollisionResult& collision);
	PRIVATE void OnCollisionExit(Collider* collider);
	PRIVATE void OnCollisionStay(const CollisionResult& collision);

	PUBLIC Body* GetBody() const;
	PUBLIC BoxCollider* GetBoxCollider() const;
	PUBLIC CircleCollider* GetCircleCollider() const;

	PUBLIC void SetWidth(float width);
	PUBLIC void SetHeight(float height);
	PUBLIC void SetFullHeight(float height);
	PUBLIC float GetWidth() const;
	PUBLIC float GetHeight() const;
	PUBLIC float GetFullHeight() const;
	PUBLIC float GetCenterToBottom() const;
	PUBLIC Vec2 GetBottom() const;
	PUBLIC void SetBottom(Vec2 position);

	PUBLIC void SetMaxGroundAngle(float angle);
	PUBLIC void SetGroundLayers(layer_t layers);
	PUBLIC void SetRayLength(float length);
	PUBLIC void SetFriction(float friction);
	PUBLIC void SetDefaultGravityScale(float scale);
	PUBLIC void SetStickGravityScale(float scale);
	PUBLIC void SetPlatformLayerIndex(unsigned int layerIndex);

	PUBLIC void SetUnrealisticPhysics(bool enable);
	PUBLIC void SetStickOnGround(bool enable);
	PUBLIC void SetIgnoreRestitute(bool enable);
	PUBLIC void SetIgnoreSlope(bool enable);
	PUBLIC void SetUseFriction(bool enable);
	PUBLIC void SetTripleRay(bool enable);

	PUBLIC void AccelerateTo(const Vec2& direction, float speed, float increase);
	PUBLIC void SetHorizontalVelocity(float hv);
	PUBLIC void SetJump(float vv);
	PUBLIC void SetJump(const Vec2& v);
	PUBLIC void CancelJumpState();
	PUBLIC void SetVelocity(const Vec2& v);
	PUBLIC void AddVelocity(const Vec2& v);
	PUBLIC Vec2 GetVeloicty() const;
	PUBLIC void ThroughPlatformOnce();
	PUBLIC bool HasGround() const;
	PUBLIC bool HasCollision() const;
	PUBLIC bool HasRayHit() const;
	PUBLIC RaycastResult GetRaycastResult() const;
	PUBLIC bool IsJumping() const;
	PUBLIC void SetThroughPlatform(bool enable);
	PUBLIC bool GetThroughPlatform() const;

	PRIVATE void MakeBody();
	PRIVATE void MakeColliders();
	PRIVATE void AttachBody(Body* body);
	PRIVATE void AttachCollider(Collider* collider);
	PRIVATE void DetachBody(Body* body);
	PRIVATE void DetachCollider(Collider* collider);

	PRIVATE bool AddGround(const CollisionResult& collision);
	PRIVATE void RemoveGround(Collider* collider);
	PRIVATE bool RaycastCheck(RaycastResult* pResult) const;
	PRIVATE void DrawRaycastCheck();
	PRIVATE void DetachFromGround();

	PRIVATE void IgnoreRestitute();
	PRIVATE void IgnoreSlope();
	PRIVATE void StickOnGround();
	PRIVATE void ApplyFriction();
	PRIVATE void ThroughPlatform();

	// �� ������Ʈ�� �����ϴ� �ٵ��Դϴ�.
	PRIVATE Body* m_body;

	// �� ������Ʈ�� �����ϴ� �ݶ��̴����Դϴ�.
	PRIVATE BoxCollider* m_boxCollider;
	PRIVATE CircleCollider* m_circleCollider;

	// �ٴ����� �ν��� �����Դϴ�.
	PRIVATE float m_maxGroundAngle;

	// �ٴ����� �ν��� ���̾���Դϴ�.
	PRIVATE layer_t m_groundLayers;

	// �浹 �̺�Ʈ�� ���� ���⿡ �ٴڵ��� ����˴ϴ�.
	PRIVATE std::vector<CollidedGround> m_grounds;

	// �ٴ� Ȯ���� ���� ������ �����Դϴ�.
	PRIVATE float m_rayLength;

	// �ٴ� Ȯ���� ���� ������ �浹 �����Դϴ�.
	PRIVATE bool m_hasRayResult;

	// �ٴ� Ȯ�ο� ������ ����Դϴ�.
	PRIVATE RaycastResult m_rayResult;

	// �ٴڿ� �������϶��� �����Դϴ�.
	PRIVATE float m_friction;

	// �⺻���� �߷� �������Դϴ�.
	PRIVATE float m_defaultGravityScale;

	// ���� ���̱� ���� �߷� �������Դϴ�.
	PRIVATE float m_stickGravityScale;

	// �������� ��Ÿ���� �÷����Դϴ�.
	PRIVATE bool m_jumping;

	// �ٴڿ� ���̱� ���� �÷����Դϴ�.
	PRIVATE bool m_stickOnGround;

	// �ٴڿ� ��� ���� �ӵ��� ���ִ� �÷����Դϴ�.
	PRIVATE bool m_ignoreRestitute;

	// �������϶� �߷��� ���ִ� �÷����Դϴ�.
	PRIVATE bool m_ignoreSlope;

	// ������ ������ ����ϴ� �÷����Դϴ�.
	PRIVATE bool m_useFriction;

	// ������ ������ �÷����� ���̾� �ε����Դϴ�.
	PRIVATE unsigned int m_platformLayerIndex;

	// Ȱ��ȭ �� ���� �÷����� �浹���� �ʽ��ϴ�.
	// �ٴ� ���̾ �浹�ϸ� �����˴ϴ�.
	PRIVATE bool m_throughPlatformOnce;

	PRIVATE float m_throughPlatformOnceElapsed;

	// ���� �������� ����ȿ���� ����ϴ� �÷����Դϴ�.
	PRIVATE bool m_unrealisticPhysicsMode;

	// ���� üũ�� ���̸� 3�� ����ϴ� �÷����Դϴ�.
	// ��Ȱ��ȭ �Ǹ� �� ���� ���̸� ����մϴ�.
	PRIVATE bool m_tripleRay;

	// Ȱ��ȭ �� ���� �÷����� �浹���� �ʽ��ϴ�.
	// �������� ������ ��� �մϴ�.
	PRIVATE bool m_throughPlatform;

	PRIVATE float m_airTime;

	// void OnGround(const Vec2& normal, const Vec2& point)
	PUBLIC Delegate<void, const Vec2&, const Vec2&> OnGround;
};

