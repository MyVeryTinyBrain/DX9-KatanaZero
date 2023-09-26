#include "base_stdafx.h"
#include "Body.h"
#include "Physics.h"
#include "Box2dHeader.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"
#include "Time.h"
#include "LayerManager.h"
#include "CoreExecutionOrder.h"

#define PHYSICS Physics
#define B2WORLD PHYSICS::GetInstance()->GetWorld()

void Body::Awake()
{
	// ���� �ùķ��̼��� ���� ���� ���� ���� ����ǵ��� �մϴ�.
	// ���� �ùķ��̼��� ���� ���� �ٷ� Ʈ�������� ���� �����ϱ� �����Դϴ�.
	m_fixedUpdateExecutionOrder = FIXED_UPDATE_ORDER_BODY;

	m_lateFixedUpdateExecutionOrder = LATE_FIXED_UPDATE_ORDER_BODY;

	m_updateExecutionOrder = UPDATE_ORDER_BODY;

	m_lateUpdateExecutionOrder = LATE_UPDATE_ORDER_BODY;

	// ��� ó���� ���� �� ����ǵ��� �մϴ�.
	// ��� ó���� ���� �� �ٵ� Ʈ�������� ���� �����ϱ� �����Դϴ�.
	m_afterUpdateExecutionOrder = AFTER_UPDATE_ORDER_BODY;

	b2BodyDef bodyDef;
	bodyDef.allowSleep = true;
	bodyDef.angle = GetTransform()->GetAngle() * DEG2RAD;
	bodyDef.angularDamping = 0.00f;
	bodyDef.awake = true;
	bodyDef.bullet = false;
	bodyDef.enabled = GetGameObject()->GetActive();
	bodyDef.fixedRotation = false;
	bodyDef.gravityScale = 1.0f;
	bodyDef.linearDamping = 0.0f;
	bodyDef.position = VEC2_TO_B2VEC2(GetTransform()->GetPosition());
	bodyDef.type = b2BodyType::b2_dynamicBody;
	// b2Body�� ���������Ϳ� �� ��ü�� �����մϴ�.
	// b2Body <--> Body ���·� ������ �����մϴ�.
	bodyDef.userData.pointer = uintptr_t(this);
	m_body = B2WORLD->CreateBody(&bodyDef);
	
	// �е��� ��Ÿ���ϴ�.
	// ���� Ŭ���� �� ���ſ��� ��Ÿ���� ������ �� �����ϴ�.
	m_density = 1;

	// �� �ٵ��� ���̾��Դϴ�.
	// �⺻ ���̾�� 0�̸�
	// �� ���̾�� ������ �����ϱ� �������� ��� ���̾�� �浹�մϴ�.
	m_layerIndex = 0;

	// ���̾� �浹 ���� �̺�Ʈ�� �޽��ϴ�.
	LayerManager::OnLayerChanged += Function<void>(this, &Body::OnLayerChanged);

	UpdateTransformComponent2Body();
	SetInterpolation(true);
}

void Body::Start()
{
	UpdateTransformComponent2Body();
	if (m_interpolation) BackupTransform();
}

void Body::BeforeFixedStep()
{
	if (m_interpolation) RollbackTransform();
}

void Body::FixedUpdate()
{
	UpdateTransformBody2Component();
	if (m_interpolation) BackupTransform();
}

void Body::LateFixedUpdate()
{
	UpdateTransformComponent2Body();
	if (m_interpolation) BackupTransform();
}

void Body::Update()
{
	if (m_interpolation) Interpolate();
}

void Body::LateUpdate()
{
	//if (!m_interpolation) UpdateTransformComponent2Body();
}

void Body::AfterUpdate()
{
	if (m_interpolation) UpdateBackupTransform();
	else UpdateTransformComponent2Body();
}

void Body::OnEnable()
{
	UpdateBodyEnable();
}

void Body::OnDisable()
{
	UpdateBodyEnable();
}

void Body::OnEnabledGameObject()
{
	UpdateBodyEnable();
}

void Body::OnDisabledGameObject()
{
	UpdateBodyEnable();
}

void Body::OnDestroy()
{
	// ���̾� �浹 ���� �̺�Ʈ�� ���� �ʽ��ϴ�.
	LayerManager::OnLayerChanged -= Function<void>(this, &Body::OnLayerChanged);
	
	// �� ������Ʈ�� ���ŵ� �� ���忡�� �ٵ� �ı��մϴ�.
	B2WORLD->DestroyBody(m_body);
}

void Body::OnLayerChanged()
{
	// ���̾� �Ŵ������� ���̾� �浹�� ����� ��� ȣ��˴ϴ�.
	// ���̾� �浹�� �ٽ� �����մϴ�.
	SetLayerIndex(m_layerIndex);
}

void Body::AttachCollider(Collider* collider)
{
	// �̹� ������ �ݶ��̴��� �� �����Ϸ��� ��� �����մϴ�.
	if (IsContains(collider))
	{
		return;
	}

	collider->AttachToBody(this);
}

void Body::DetachCollider(Collider* collider)
{
	// �������� ���� �ݶ��̴��� Ż���Ϸ��� ��� �����մϴ�.
	if (!IsContains(collider))
	{
		return;
	}

	collider->DetachFromBody();
}

size_t Body::GetColliderCount() const
{
	return m_body->GetFixtureCount();
}

Collider* Body::GetCollider(size_t index) const
{
	b2Fixture* fixture = m_body->GetFixtureList();
	for (size_t i = 0; i < index; ++i)
		fixture = fixture->GetNext();
	// b2Fixture�� ���������Ϳ��� Collider ��ü�� ����Ǿ� �ֽ��ϴ�.
	// b2Fixture <--> Collider ������ ������ �����ϹǷ� ����ȯ�Ͽ� ��ȯ�մϴ�.
	return (Collider*)(fixture->GetUserData().pointer);
}

bool Body::IsContains(Collider* collider) const
{
	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture)
	{
		Collider* current = (Collider*)(fixture->GetUserData().pointer);
		if (current == collider)
		{
			return true;
		}
		fixture = fixture->GetNext();
	}

	return false;
}

void Body::SetPosition(const Vec2& position)
{
	m_body->SetTransform(VEC2_TO_B2VEC2(position), GetTransform()->GetAngle());
	GetTransform()->SetPosition(position);
}

void Body::SetAngle(float angle)
{
	m_body->SetTransform(VEC2_TO_B2VEC2(GetTransform()->GetPosition()), angle * DEG2RAD);
	GetTransform()->SetAngle(angle);
}

void Body::AddForce(const Vec2& force)
{
	m_body->ApplyForce(VEC2_TO_B2VEC2(force), m_body->GetWorldCenter(), true);
}

void Body::AddImpulse(const Vec2& impulse)
{
	m_body->ApplyLinearImpulse(VEC2_TO_B2VEC2(impulse), m_body->GetWorldCenter(), true);
}

void Body::AddTorque(const float& torque)
{
	m_body->ApplyTorque(torque, true);
}

void Body::AddAcceleration(const Vec2& acceleration)
{
	SetVelocity(GetVelocity() + acceleration * Time::GetFixedDeltaTime());
	//m_body->SetAwake(true);
}

void Body::AddVelocity(const Vec2& velocity)
{
	SetVelocity(GetVelocity() + velocity);
	//m_body->SetAwake(true);
}

void Body::AccelerateTo(Vec2 direction, float speed, float increase)
{
	const Vec2& currentV = GetVelocity();
	float currentProj = Vec2::ProjectOnVec2(currentV, direction).magnitude();
	if (currentProj > speed) return;
	const Vec2& targetV = direction * speed;
	Vec2 increaseV = direction * increase;
	Vec2 increasedV = currentV + increaseV;
	float incrasedProj = Vec2::ProjectOnVec2(increasedV, direction).magnitude();
	if (incrasedProj > speed)
	{
		increase = speed - currentProj;
		increase = fabsf(increase);
	}
	AddVelocity(direction * increase);
}

void Body::AccelerateTo(Vec2 direction, float speed)
{	
	Vec2 targetV = direction * speed;
	AddAcceleration(targetV * GetLinearDamping());
}

void Body::UpdateInterpolation()
{
	UpdateTransformComponent2Body();
	BackupTransform();
}

void Body::SetInterpolation(bool interpolation)
{
	if (m_interpolation == interpolation) return;
	if (interpolation) BackupTransform();
	m_interpolation = interpolation;
}

void Body::SetType(BodyType type)
{
	b2BodyType b2Type = b2BodyType::b2_dynamicBody;
	switch (type)
	{
		case BodyType::Dynamic:
			b2Type = b2BodyType::b2_dynamicBody;
			break;
		case BodyType::Kinematic:
			b2Type = b2BodyType::b2_kinematicBody;
			break;
		case BodyType::Static:
			b2Type = b2BodyType::b2_staticBody;
			break;
	}

	m_body->SetType(b2Type);
	m_body->SetAwake(true);
}

BodyType Body::GetType() const
{
	BodyType type = BodyType::Dynamic;
	switch (m_body->GetType())
	{
		case b2BodyType::b2_dynamicBody:
			type = BodyType::Dynamic;
			break;
		case b2BodyType::b2_kinematicBody:
			type = BodyType::Kinematic;
			break;
		case b2BodyType::b2_staticBody:
			type = BodyType::Static;
			break;
	}
	return type;
}

void Body::SetLayerIndex(unsigned int layerIndex)
{
	m_layerIndex = layerIndex;

	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture)
	{
		Collider* current = (Collider*)(fixture->GetUserData().pointer);
		current->UpdateLayer();
		fixture = fixture->GetNext();
	}
}

unsigned int Body::GetLayerIndex() const
{
	return m_layerIndex;
}

void Body::SetDensity(float density)
{
	m_density = density;

	b2Fixture* fixture = m_body->GetFixtureList();
	while (fixture)
	{
		Collider* current = (Collider*)(fixture->GetUserData().pointer);
		current->UpdateDensity();
		fixture = fixture->GetNext();
	}
}

float Body::GetDensity() const
{
	return m_density;
}

void Body::SetGravityScale(float scale)
{
	m_body->SetGravityScale(scale);
}

float Body::GetGravityScale() const
{
	return m_body->GetGravityScale();
}

void Body::SetGravityMode(bool enable)
{
	m_body->SetGravityMode(enable);
}

bool Body::IsGravityMode() const
{
	return m_body->GetGravityMode();
}

void Body::SetVelocity(const Vec2& velocity)
{
	m_body->SetLinearVelocity(VEC2_TO_B2VEC2(velocity));
}

Vec2 Body::GetVelocity() const
{
	return B2VEC2_TO_VEC2(m_body->GetLinearVelocity());
}

void Body::SetAngularVelocity(float& angularVelocity)
{
	m_body->SetAngularVelocity(angularVelocity);
}

float Body::GetAngularVelocity() const
{
	return m_body->GetAngularVelocity();
}

void Body::SetFixedRotationMode(bool enable)
{
	m_body->SetFixedRotation(enable);
}

bool Body::IsFixedRotationMode() const
{
	return m_body->IsFixedRotation();
}

void Body::SetLinearDamping(float damping)
{
	m_body->SetLinearDamping(damping);
}

float Body::GetLinearDamping() const
{
	return m_body->GetLinearDamping();
}

void Body::SetAngularDamping(float damping)
{
	m_body->SetAngularDamping(damping);
}

float Body::GetAngularDamping() const
{
	return m_body->GetAngularDamping();
}

void Body::SetContinuousMode(bool enable)
{
	m_body->SetBullet(enable);
}

bool Body::IsContinuousMode() const
{
	return m_body->IsBullet();
}

void Body::SetAlwaysWakeMode(bool enable)
{
	m_body->SetSleepingAllowed(!enable);
}

bool Body::IsAlwaysWakeMode() const
{
	return !m_body->IsSleepingAllowed();
}

void Body::UpdateBodyEnable()
{
	// ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ��ְ�, ������Ʈ�� Ȱ��ȭ �Ǿ������� �÷��װ� �����ϴ�.
	bool enable = GetGameObject()->GetActive() && GetEnable();

	// �ٵ��� �÷��׿� ���� �÷��װ� ���ٸ� �����մϴ�.
	if (enable == m_body->IsEnabled()) return;

	// �ٵ��� Ȱ��ȭ �÷��׸� �����մϴ�.
	m_body->SetEnabled(enable);
}

void Body::UpdateTransformComponent2Body()
{
	// ������Ʈ�� ���� �ٵ� �����մϴ�.
	Transform* transform = GetTransform();
	const Vec2& position = transform->GetPosition();
	float angle = transform->GetAngle();
	b2Vec2 b2Position = VEC2_TO_B2VEC2(position);
	float b2Angle = angle * DEG2RAD;

	if (m_body->GetPosition() != b2Position || m_body->GetAngle() != b2Angle)
	{
		m_body->SetTransform(b2Position, b2Angle);
		m_body->SetAwake(true);
	}
}

void Body::UpdateTransformBody2Component()
{
	// �ùķ��̼� ���� Ʈ�������� �����մϴ�.
	if (m_body->GetType() != b2BodyType::b2_dynamicBody) return;
	GetTransform()->SetPosition(B2VEC2_TO_VEC2(m_body->GetPosition()));
	GetTransform()->SetAngle(m_body->GetAngle() * RAD2DEG);
}

void Body::BackupTransform()
{
	m_backupPosition = B2VEC2_TO_VEC2(m_body->GetPosition());
	m_backupAngle = m_body->GetAngle() * RAD2DEG;
}

void Body::UpdateBackupTransform()
{
	Vec2 lerpPosition = m_backupPosition + GetVelocity() * Time::GetFixedUpdateAccumulated() * Time::GetFixedTimeScale();
	float lerpAngle = m_backupAngle + GetAngularVelocity() * Time::GetFixedUpdateAccumulated() * Time::GetFixedTimeScale();
	Vec2 curPosition = GetTransform()->GetPosition();
	float curAngle = GetTransform()->GetAngle();
	
	Vec2 deltaPosition = curPosition - lerpPosition;
	float deltaAngle = curAngle - lerpAngle;

	m_backupPosition += deltaPosition;
	m_backupAngle += deltaAngle;
}

void Body::RollbackTransform()
{
	b2Vec2 b2Position = VEC2_TO_B2VEC2(m_backupPosition);
	float b2Angle = m_backupAngle * DEG2RAD;

	//if (m_body->GetPosition() != b2Position || m_body->GetAngle() != b2Angle)
	//{
	//	m_body->SetTransform(b2Position, b2Angle);
	//	m_body->SetAwake(true);
	//}

	float deltaDist = Vec2::Distance(m_backupPosition, B2VEC2_TO_VEC2(m_body->GetPosition()));
	float deltaAngle = fabsf(b2Angle - m_body->GetAngle() != b2Angle);

	if (deltaDist > b2_linearSlop * 10 + 0.01f || deltaAngle > b2_angularSlop * 20 + 0.01f * RAD2DEG)
	{
		m_body->SetTransform(b2Position, b2Angle);
		m_body->SetAwake(true);
	}
}

void Body::Interpolate()
{
	Vec2 position = m_backupPosition + GetVelocity() * Time::GetFixedUpdateAccumulated() * Time::GetFixedTimeScale();
	float angle = m_backupAngle + GetAngularVelocity() * Time::GetFixedUpdateAccumulated() * Time::GetFixedTimeScale();
	b2Vec2 b2Position = VEC2_TO_B2VEC2(position);
	float b2Angle = angle * DEG2RAD;

	if (m_body->GetPosition() != b2Position || m_body->GetAngle() != b2Angle)
	{
		m_body->SetTransform(b2Position, b2Angle);
		m_body->SetAwake(true);

		GetTransform()->SetPosition(position);
		GetTransform()->SetAngle(angle);
	}
}
