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

	// 이 컴포넌트가 관리하는 바디입니다.
	PRIVATE Body* m_body;

	// 이 컴포넌트가 관리하는 콜라이더들입니다.
	PRIVATE BoxCollider* m_boxCollider;
	PRIVATE CircleCollider* m_circleCollider;

	// 바닥으로 인식할 각도입니다.
	PRIVATE float m_maxGroundAngle;

	// 바닥으로 인식할 레이어들입니다.
	PRIVATE layer_t m_groundLayers;

	// 충돌 이벤트를 통해 여기에 바닥들이 저장됩니다.
	PRIVATE std::vector<CollidedGround> m_grounds;

	// 바닥 확인을 위한 레이의 길이입니다.
	PRIVATE float m_rayLength;

	// 바닥 확인을 위한 레이의 충돌 여부입니다.
	PRIVATE bool m_hasRayResult;

	// 바닥 확인용 레이의 결과입니다.
	PRIVATE RaycastResult m_rayResult;

	// 바닥에 접지중일때의 감쇠입니다.
	PRIVATE float m_friction;

	// 기본적인 중력 스케일입니다.
	PRIVATE float m_defaultGravityScale;

	// 땅에 붙이기 위한 중력 스케일입니다.
	PRIVATE float m_stickGravityScale;

	// 점프중을 나타내는 플래그입니다.
	PRIVATE bool m_jumping;

	// 바닥에 붙이기 위한 플래그입니다.
	PRIVATE bool m_stickOnGround;

	// 바닥에 닿는 순간 속도를 없애는 플래그입니다.
	PRIVATE bool m_ignoreRestitute;

	// 접지중일때 중력을 없애는 플래그입니다.
	PRIVATE bool m_ignoreSlope;

	// 접지중 마찰을 사용하는 플래그입니다.
	PRIVATE bool m_useFriction;

	// 관통이 가능한 플랫폼의 레이어 인덱스입니다.
	PRIVATE unsigned int m_platformLayerIndex;

	// 활성화 된 동안 플랫폼과 충돌하지 않습니다.
	// 바닥 레이어에 충돌하면 해제됩니다.
	PRIVATE bool m_throughPlatformOnce;

	PRIVATE float m_throughPlatformOnceElapsed;

	// 여러 비현실적 물리효과를 사용하는 플래그입니다.
	PRIVATE bool m_unrealisticPhysicsMode;

	// 접지 체크용 레이를 3번 사용하는 플래그입니다.
	// 비활성화 되면 한 번만 레이를 사용합니다.
	PRIVATE bool m_tripleRay;

	// 활성화 된 동안 플랫폼과 충돌하지 않습니다.
	// 수동으로 설정해 줘야 합니다.
	PRIVATE bool m_throughPlatform;

	PRIVATE float m_airTime;

	// void OnGround(const Vec2& normal, const Vec2& point)
	PUBLIC Delegate<void, const Vec2&, const Vec2&> OnGround;
};

