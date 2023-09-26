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

	// ���� �ٵ� �����Ǿ����� ������ ��Ÿ���ϴ�.
	m_body = nullptr;

	UpdateShape(&m_shape, m_prevLocalPositionFromBody, m_prevLocalAngleFromBody, m_prevLocalScaleFromBody);

	DrawDebug = true;

	// Ʈ���� �θ� �ٵ� �ִٸ� �� �ٵ� �ٽ��ϴ�.
	AutoAttach();

	// �ٵ� �������� �� ���� Ʈ�������� ��ȭ�Ǿ��ٸ� ����� �缳���ϰ� �Ƚ��ĸ� �ı� �� �ٽ� �����մϴ�.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::Start()
{
	// Ʈ���� �θ� �ٵ� �ִٸ� �� �ٵ� �ٽ��ϴ�.
	AutoAttach();

	// �ٵ� �������� �� ���� Ʈ�������� ��ȭ�Ǿ��ٸ� ����� �缳���ϰ� �Ƚ��ĸ� �ı� �� �ٽ� �����մϴ�.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::FixedUpdate()
{
	if (!m_body) return;
	if (!m_body->GetActive()) return;

	// �ٵ� �������� �� ���� Ʈ�������� ��ȭ�Ǿ��ٸ� ����� �缳���ϰ� �Ƚ��ĸ� �ı� �� �ٽ� �����մϴ�.
	if (CheckLocalChanged())
	{
		UpdateCollider();
	}
}

void Collider::LateUpdate()
{
	if (!m_body) return;
	if (!m_body->GetActive()) return;

	// �ٵ� �������� �� ���� Ʈ�������� ��ȭ�Ǿ��ٸ� ����� �缳���ϰ� �Ƚ��ĸ� �ı� �� �ٽ� �����մϴ�.
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
	// �� �ݶ��̴��� �ٵ� �����Ǿ� �ִٸ�
	// �ٵ𿡼� Ż����Ű�� �Ƚ��ĸ� �ı��մϴ�.
	DetachFromBody();

	SAFE_DELETE(m_shape);
}

void Collider::OnDestroyBody(Object* object)
{
	// �ٵ� �����Ǿ� �ִٸ�
	// �ٵ� �ı��Ǳ� ���� �ٵ𿡼� Ż����Ű�� �Ƚ��ĸ� �ı��մϴ�.

	Body* body = static_cast<Body*>(object);

	// Ȥ�ö� �ı��Ǵ� �ٵ� �����Ǿ� �ִ� �ٵ� �ƴѰ�� �����մϴ�.
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

	// ���� �����Ӱ� ���� �������� ���� ��ȯ�� ���̰� ���������� ū ��� Ȱ��ȭ�˴ϴ�.
	bool localChanged = false;

	// ���̰� �ִ� ��� ���� ���� Ʈ������ ������ ������Ʈ�մϴ�.

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

	// ���� �Ƚ��İ� ������ ���¶��
	// �� �ݶ��̴��� ������ �Ƚ��ĸ� �����մϴ�.
	if (m_fixture)
	{
		m_body->m_body->DestroyFixture(m_fixture);
		m_fixture = nullptr;
	}

	b2FixtureDef fixtureDef;
	CreateFixtureDef(&fixtureDef);
	// �Ƚ����� ���� �����Ϳ� �� ��ü�� �����մϴ�.
	// b2Fixture <--> Collider ������ ������ �����մϴ�.
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
	// ���� �ٵ� ���� �� �����Ƿ��� ��쿡�� �����մϴ�.
	if (body == m_body)
	{
		return;
	}

	// �ٸ� �ٵ� �����Ƿ��� ��쿡�� ���� �����Ǿ� �ִ� �ٵ𿡼� Ż���մϴ�.
	// Ż�� �� �����ϰ� �˴ϴ�.
	if (m_body && body != m_body)
	{
		DetachFromBody();
	}
	
	// ������ �ٵ� �����մϴ�.
	m_body = body;

	// �ٵ�κ����� ���� ��ǥ�� ������Ʈ�մϴ�.
	CheckLocalChanged();

	// �Ƚ��ĸ� �����ϰ� �ٵ� �����մϴ�.
	UpdateCollider();

	// �ٵ� �ı��Ǵ� �̺�Ʈ�� �����մϴ�.
	body->OnDestoryCallback += Function<void, Object*>(this, &Collider::OnDestroyBody);
}

void Collider::DetachFromBody()
{
	// ��� �ٵ𿡵� �����Ǿ� ���� �ʴٸ� �����մϴ�.
	if (!m_body)
	{
		return;
	}
	
	// �ٵ� �ı��Ǵ� �̺�Ʈ ������ �����մϴ�.
	m_body->OnDestoryCallback -= Function<void, Object*>(this, &Collider::OnDestroyBody);

	// ������ �Ƚ��ĸ� �ٵ𿡼� �����ϰ� �ı��մϴ�.
	m_body->m_body->DestroyFixture(m_fixture);

	// ������ �Ƚ��İ� ���ٰ� ��Ÿ���ϴ�.
	m_fixture = nullptr;

	// ������ �ٵ� ������ ��Ÿ���ϴ�.
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
	// �ٵ� nullptr�� �ƴ϶�� �����Ǿ� �ִ� �����Դϴ�.
	// �ٵ� nullptr�̶�� �����Ǿ� ���� ���� �����Դϴ�.
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
