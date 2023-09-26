#include "base_stdafx.h"
#include "Collider.h"
#include "Box2dHeader.h"
#include "Transform.h"
#include "Body.h"
#include "LayerManager.h"
#include "Box2dHeader.h"
#include "CoreExecutionOrder.h"

#define TRANSFORM (GetTransform())

void Collider::Awake()
{
	m_fixedUpdateExecutionOrder = FIXED_UPDATE_ORDER_COLLIDER,
	m_updateExecutionOrder = UPDATE_ORDER_COLLIDER;
	m_lateUpdateExecutionOrder = LATE_UPDATE_ORDER_COLLIDER;

	m_shape = nullptr;
	m_fixture = nullptr;

	m_prevLocalPositionFromBody = {0, 0};
	m_prevLocalAngleFromBody = 0;
	m_prevLocalScaleFromBody = {1, 1};

	m_restitution = 0.0f;
	m_friction = 0.0f;
	m_isTrigger = false;

	m_independenceLayerIndexMode = false;
	m_independenceLayerIndex = 0;

	m_ignoreLayer = 0x00;

	// 현재 바디에 부착되어있지 않음을 나타냅니다.
	m_body = nullptr;

	UpdateShape(&m_shape, m_prevLocalPositionFromBody, m_prevLocalAngleFromBody, m_prevLocalScaleFromBody);

	DrawDebug = true;

	// 트리의 부모에 바디가 있다면 그 바디에 붙습니다.
	AutoAttach();

	// 바디를 기준으로 한 로컬 트랜스폼이 변화되었다면 모양을 재설정하고 픽스쳐를 파괴 후 다시 생성합니다.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::Start()
{
	// 트리의 부모에 바디가 있다면 그 바디에 붙습니다.
	AutoAttach();

	// 바디를 기준으로 한 로컬 트랜스폼이 변화되었다면 모양을 재설정하고 픽스쳐를 파괴 후 다시 생성합니다.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::FixedUpdate()
{
	if (!m_body) return;
	if (!m_body->GetActive()) return;

	// 바디를 기준으로 한 로컬 트랜스폼이 변화되었다면 모양을 재설정하고 픽스쳐를 파괴 후 다시 생성합니다.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::LateUpdate()
{
	if (!m_body) return;
	if (!m_body->GetActive()) return;

	// 바디를 기준으로 한 로컬 트랜스폼이 변화되었다면 모양을 재설정하고 픽스쳐를 파괴 후 다시 생성합니다.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::OnEnable()
{
	UpdateLayer();
}

void Collider::OnDisable()
{
	UpdateLayer();
}

void Collider::OnEnabledGameObject()
{
	UpdateLayer();
}

void Collider::OnDisabledGameObject()
{
	UpdateLayer();
}

void Collider::OnDestroy()
{
	// 이 콜라이더가 바디에 부착되어 있다면
	// 바디에서 탈착시키고 픽스쳐를 파괴합니다.
	DetachFromBody();

	SAFE_DELETE(m_shape);
}

void Collider::OnDestroyBody(Object* object)
{
	// 바디에 부착되어 있다면
	// 바디가 파괴되기 전에 바디에서 탈착시키고 픽스쳐를 파괴합니다.

	Body* body = static_cast<Body*>(object);

	// 혹시라도 파괴되는 바디가 부착되어 있는 바디가 아닌경우 종료합니다.
	if (body != m_body)
	{
		return;
	}

	DetachFromBody();
}

bool Collider::CalculateLocalFromBody(Vec2* localPositionFromBody, float* localAngleFromBody, Vec2* localScaleFromBody)
{
	const Transform* transform = GetTransform();
	const Vec2& position = transform->GetPosition();
	const float& angle = transform->GetAngle();
	const Vec2& scale = transform->GetScale();

	if (!m_body)
	{
		*localPositionFromBody = position;
		*localAngleFromBody = angle;
		*localScaleFromBody = scale;
		return false;
	}

	const Transform* bodyTransform = m_body->GetTransform();
	const Mat4& worldToBodyLocalMatrix = bodyTransform->WorldToLocalMatrix();
	const float& bodyWorldAngle = bodyTransform->GetAngle();
	const Vec2& bodyWorldScale = bodyTransform->GetScale();

	*localPositionFromBody = Vec4(position.x, position.y, 0, 1) * worldToBodyLocalMatrix;
	*localAngleFromBody = angle - bodyWorldAngle;
	//*localScaleFromBody = { scale.x / bodyWorldScale.x, scale.y / bodyWorldScale.y };
	*localScaleFromBody = scale;
	return true;
}

bool Collider::CheckLocalChanged()
{
	if (!m_body)
	{
		return false;
	}

	Vec2 localPositionFromBody;
	float localAngleFromBody;
	Vec2 localScaleFromBody;
	CalculateLocalFromBody(&localPositionFromBody, &localAngleFromBody, &localScaleFromBody);

	// 이전 프레임과 현재 프레임의 로컬 변환의 차이가 지정값보다 큰 경우 활성화됩니다.
	bool localChanged = false;

	// 차이가 있는 경우 이전 로컬 트랜스폼 변수를 업데이트합니다.

	Vec2 deltaLocalPosition = localPositionFromBody - m_prevLocalPositionFromBody;
	float deltaLocalPositionDistance = deltaLocalPosition.magnitude();
	if (deltaLocalPositionDistance > 0.01f)
	{
		localChanged = true;
		m_prevLocalPositionFromBody = localPositionFromBody;
	}

	float deltaAbsLocalAngle = fabsf(localAngleFromBody - m_prevLocalAngleFromBody);
	if (deltaAbsLocalAngle > 0.01f)
	{
		localChanged = true;
		m_prevLocalAngleFromBody = localAngleFromBody;
	}

	Vec2 deltaLocalScale = localScaleFromBody - m_prevLocalScaleFromBody;
	float deltaLocalScaleDistance = deltaLocalScale.magnitude();
	if (deltaLocalScaleDistance > 0.01f)
	{
		localChanged = true;
		m_prevLocalScaleFromBody = localScaleFromBody;
	}

	return localChanged;
}

void Collider::UpdateFixture()
{
	if (!m_body) return;

	// 현재 픽스쳐가 생성된 상태라면
	// 이 콜라이더가 생성한 픽스쳐를 제거합니다.
	if (m_fixture)
	{
		m_body->m_body->DestroyFixture(m_fixture);
		m_fixture = nullptr;
	}

	b2FixtureDef fixtureDef;
	CreateFixtureDef(&fixtureDef);
	// 픽스쳐의 유저 데이터에 이 객체를 저장합니다.
	// b2Fixture <--> Collider 형태의 접근이 가능합니다.
	fixtureDef.userData.pointer = uintptr_t(this);
	m_fixture = m_body->m_body->CreateFixture(&fixtureDef);
}

void Collider::UpdateCollider()
{
	UpdateShape(&m_shape, m_prevLocalPositionFromBody, m_prevLocalAngleFromBody, m_prevLocalScaleFromBody);
	UpdateFixture();
}

void Collider::AttachToBody(Body* body)
{
	// 같은 바디에 여러 번 부착되려는 경우에는 종료합니다.
	if (body == m_body)
	{
		return;
	}

	// 다른 바디에 부착되려는 경우에는 현재 부착되어 있는 바디에서 탈착합니다.
	// 탈착 후 부착하게 됩니다.
	if (m_body && body != m_body)
	{
		DetachFromBody();
	}
	
	// 부착된 바디를 저장합니다.
	m_body = body;

	// 바디로부터의 로컬 좌표를 업데이트합니다.
	CheckLocalChanged();

	// 픽스쳐를 생성하고 바디에 부착합니다.
	UpdateCollider();

	// 바디가 파괴되는 이벤트를 수신합니다.
	body->OnDestoryCallback += Function<void, Object*>(this, &Collider::OnDestroyBody);
}

void Collider::DetachFromBody()
{
	// 어떠한 바디에도 부착되어 있지 않다면 종료합니다.
	if (!m_body)
	{
		return;
	}
	
	// 바디가 파괴되는 이벤트 수신을 종료합니다.
	m_body->OnDestoryCallback -= Function<void, Object*>(this, &Collider::OnDestroyBody);

	// 생성한 픽스쳐를 바디에서 제거하고 파괴합니다.
	m_body->m_body->DestroyFixture(m_fixture);

	// 생성한 픽스쳐가 없다고 나타냅니다.
	m_fixture = nullptr;

	// 부착된 바디가 없음을 나타냅니다.
	m_body = nullptr;
}

void Collider::AutoAttach()
{
	if (!m_body)
	{
		Body* parentBody = GetComponentInParent<Body>();
		if (parentBody)
		{
			AttachToBody(parentBody);
		}
	}
}

Body* Collider::GetBody() const
{
	// 바디가 nullptr이 아니라면 부착되어 있는 상태입니다.
	// 바디가 nullptr이라면 부착되어 있지 않은 상태입니다.
	return m_body;
}

void Collider::UpdateDensity()
{
	if (!m_fixture) return;
	m_fixture->SetDensity(m_body->GetDensity());
}

void Collider::UpdateLayer()
{
	if (!m_fixture) return;
	b2Filter filter;
	CreateFilter(&filter);
	m_fixture->SetFilterData(filter);
}

float Collider::GetFriction() const
{
	return m_friction;
}

void Collider::SetFriction(float friction)
{
	m_friction = friction;
	if (!m_fixture) return;
	m_fixture->SetFriction(friction);
}

float Collider::GetRestitution() const
{
	return m_restitution;
}

void Collider::SetRestitution(float restitution)
{
	m_restitution = restitution;
	if (!m_fixture) return;
	m_fixture->SetRestitution(restitution);
}

bool Collider::IsTriggerMode() const
{
	return m_isTrigger;
}

void Collider::SetTriggerMode(bool enable)
{
	m_isTrigger = enable;
	if (!m_fixture) return;
	m_fixture->SetSensor(enable);
}

layer_t Collider::GetLayer() const
{
	return m_fixture->GetFilterData().categoryBits;
}

bool Collider::IsIndependenceLayerIndexMode() const
{
	return m_independenceLayerIndexMode;
}

void Collider::SetIndependenceLayerIndexMode(bool enable)
{
	m_independenceLayerIndexMode = enable;
	UpdateLayer();
}

unsigned int Collider::GetIndependenceLayerIndex() const
{
	return m_independenceLayerIndex;
}

void Collider::SetIndependenceLayerIndex(unsigned int layerIndex)
{
	m_independenceLayerIndex = layerIndex;
	UpdateLayer();
}

layer_t Collider::GetIgnoreLayer() const
{
	return layer_t();
}

bool Collider::GetIgnoreState(unsigned int layerIndex) const
{
	return (m_ignoreLayer & (1 << layerIndex)) > 0;
}

void Collider::SetIgnoreLayerIndex(unsigned int layerIndex, bool enable)
{
	if (enable) m_ignoreLayer |= 1 << layerIndex;
	else m_ignoreLayer &= ~(1 << layerIndex);
	UpdateLayer();
}

void Collider::SetIgnoreLayer(layer_t layer)
{
	m_ignoreLayer = layer;
}

void Collider::CreateFixtureDef(b2FixtureDef* def) const
{
	def->shape = m_shape;
	def->density = m_body->m_density;
	def->friction = m_friction;
	def->isSensor = m_isTrigger;
	def->restitution = m_restitution;
	b2Filter filter;
	CreateFilter(&filter);
	def->filter = filter;
}

void Collider::CreateFilter(b2Filter* filter) const
{
	unsigned layer;
	if (m_independenceLayerIndexMode) layer = m_independenceLayerIndex;
	else layer = m_body->m_layerIndex;

	filter->categoryBits = LayerManager::GetInstance()->GetLayer(layer);

	uint16 collisionBits = LayerManager::GetInstance()->GetCollisionLayer(layer);;
	uint16 xorBits = collisionBits ^ m_ignoreLayer;
	uint16 mask = xorBits & collisionBits;
	filter->maskBits = mask;

	filter->groupIndex = 0;
}
