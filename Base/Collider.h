#pragma once

#include "Component.h"
#include "Box2dStruct.h"

class Body;
class b2Shape;
class b2Fixture;
struct b2Filter;
struct b2FixtureDef;

class Collider : public Component
{
	friend class Body;
	friend class Physics;

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(OnEnable);
	COMPONENT_OVERRIDE(OnDisable);
	COMPONENT_OVERRIDE(OnEnabledGameObject);
	COMPONENT_OVERRIDE(OnDisabledGameObject);
	COMPONENT_OVERRIDE(OnDestroy);
	
	PRIVATE void OnDestroyBody(Object* object);

	// ================================================================

	PRIVATE bool CalculateLocalFromBody(
		Vec2* localPositionFromBody,
		float* localAngleFromBody,
		Vec2* localScaleFromBody);
	PRIVATE bool CheckLocalChanged();
	PROTECTED virtual void UpdateShape(
		b2Shape** ppShape, 
		const Vec2& localPositionFromBody,
		const float& localAngleFromBody,
		const Vec2& localScaleFromBody) = 0;
	PRIVATE void UpdateFixture();
	PUBLIC void UpdateCollider();

	PRIVATE void AttachToBody(Body* body);
	PRIVATE void DetachFromBody();

	// ================================================================

	PUBLIC Body* GetBody() const;

	PUBLIC void UpdateDensity();
	PUBLIC void UpdateLayer();

	PUBLIC float GetFriction() const;
	PUBLIC void SetFriction(float friction);
	PUBLIC float GetRestitution() const;
	PUBLIC void SetRestitution(float restitution);
	PUBLIC bool IsTriggerMode() const;
	PUBLIC void SetTriggerMode(bool enable);
	PUBLIC layer_t GetLayer() const;

	PUBLIC bool IsIndependenceLayerIndexMode() const;
	PUBLIC void SetIndependenceLayerIndexMode(bool enable);
	PUBLIC unsigned int GetIndependenceLayerIndex() const;
	PUBLIC void SetIndependenceLayerIndex(unsigned int layerIndex);

	PUBLIC layer_t GetIgnoreLayer() const;
	PUBLIC bool GetIgnoreState(unsigned int layerIndex) const;
	PUBLIC void SetIgnoreLayerIndex(unsigned int layerIndex, bool enable);
	PUBLIC void SetIgnoreLayer(layer_t layer);

	PUBLIC void AutoAttach();

	PRIVATE void CreateFixtureDef(b2FixtureDef* def) const;
	PRIVATE void CreateFilter(b2Filter* filter) const;

	// ================================================================

	PROTECTED b2Shape* m_shape;
	PRIVATE b2Fixture* m_fixture;

	PRIVATE Vec2 m_prevLocalPositionFromBody;
	PRIVATE float m_prevLocalAngleFromBody;
	PRIVATE Vec2 m_prevLocalScaleFromBody;

	PRIVATE float m_restitution;
	PRIVATE float m_friction;
	PRIVATE bool m_isTrigger;

	PRIVATE Body* m_body;

	PUBLIC bool DrawDebug;

	// 실행 순서는
	// Enter --> Exit --> Stay --> FixedUpdate 
	// 입니다.

	PUBLIC Delegate<void, const CollisionResult&> OnCollisionEnter;
	PUBLIC Delegate<void, Collider*> OnTriggerEnter;
	PUBLIC Delegate<void, Collider*> OnCollisionExit;
	PUBLIC Delegate<void, Collider*> OnTriggerExit;

	// Sleep 상태이면 호출되지 않습니다.

	PUBLIC Delegate<void, const CollisionResult&> OnCollisionStay;
	PUBLIC Delegate<void, Collider*> OnTriggerStay;

	// 이 플래그가 활성화되면 바디의 레이어 대신 독립적인 레이어를 사용합니다.

	PRIVATE bool m_independenceLayerIndexMode;
	PRIVATE unsigned int m_independenceLayerIndex;

	// 설정된 레이어들과 충돌하지 않습니다.
	// 각 비트들의 위치가 충돌하지 않을 레이어의 인덱스입니다.

	PRIVATE layer_t m_ignoreLayer;
};

