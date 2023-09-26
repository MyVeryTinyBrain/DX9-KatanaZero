#pragma once

#include "Component.h"

class b2Body;
class Collider;

enum class BodyType
{
	Dynamic,
	Kinematic,
	Static,
};

class Body : public Component
{
	friend class Collider;

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(BeforeFixedStep);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(LateFixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(AfterUpdate);
	COMPONENT_OVERRIDE(OnEnable);
	COMPONENT_OVERRIDE(OnDisable);
	COMPONENT_OVERRIDE(OnEnabledGameObject);
	COMPONENT_OVERRIDE(OnDisabledGameObject);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE void OnLayerChanged();

	// ================================================================

	// �ݶ��̴��� �����ϴٸ� �ٵ�� ���� ������Ʈ�� �����ϰų�
	// �ٵ��� �ڽ� ������Ʈ�� �����ؾ� �մϴ�.
	// �ٵ𺸴� ���� �ܰ迡 �ְų�, �ٸ� ������Ʈ�� �ִ� ��쿡��
	// �ݶ��̴��� �ùٸ� Ʈ���������� �������� ���� �� �ֽ��ϴ�.

	PUBLIC void AttachCollider(Collider* collider);
	PUBLIC void DetachCollider(Collider* collider);
	PUBLIC size_t GetColliderCount() const;
	PUBLIC Collider* GetCollider(size_t index) const;
	PUBLIC bool IsContains(Collider* collider) const;

	// ================================================================

	PUBLIC void SetPosition(const Vec2& position);
	PUBLIC void SetAngle(float angle);

	PUBLIC void AddForce(const Vec2& force);
	PUBLIC void AddImpulse(const Vec2& impulse);
	PUBLIC void AddTorque(const float& torque);
	PUBLIC void AddAcceleration(const Vec2& acceleration);
	PUBLIC void AddVelocity(const Vec2& velocity);
	PUBLIC void AccelerateTo(Vec2 direction, float speed, float increase);
	PUBLIC void AccelerateTo(Vec2 direction, float speed);

	// ================================================================

	PUBLIC void UpdateInterpolation();
	PUBLIC void SetInterpolation(bool interpolation);
	PUBLIC void SetType(BodyType type);
	PUBLIC BodyType GetType() const;
	PUBLIC void SetLayerIndex(unsigned int layerIndex);
	PUBLIC unsigned int GetLayerIndex() const;
	PUBLIC void SetDensity(float density);
	PUBLIC float GetDensity() const;
	PUBLIC void SetGravityScale(float scale);
	PUBLIC float GetGravityScale() const;
	PUBLIC void SetGravityMode(bool enable);
	PUBLIC bool IsGravityMode() const;
	PUBLIC void SetVelocity(const Vec2& velocity);
	PUBLIC Vec2 GetVelocity() const;
	PUBLIC void SetAngularVelocity(float& angularVelocity);
	PUBLIC float GetAngularVelocity() const;
	PUBLIC void SetFixedRotationMode(bool enable);
	PUBLIC bool IsFixedRotationMode() const;
	PUBLIC void SetLinearDamping(float damping);
	PUBLIC float GetLinearDamping() const;
	PUBLIC void SetAngularDamping(float damping);
	PUBLIC float GetAngularDamping() const;
	PUBLIC void SetContinuousMode(bool enable);
	PUBLIC bool IsContinuousMode() const;
	PUBLIC void SetAlwaysWakeMode(bool enable);
	PUBLIC bool IsAlwaysWakeMode() const;

	// ================================================================

	PRIVATE void UpdateBodyEnable();

	PRIVATE void UpdateTransformComponent2Body();
	PRIVATE void UpdateTransformBody2Component();

	PRIVATE void BackupTransform();
	PRIVATE void UpdateBackupTransform();
	PRIVATE void RollbackTransform();
	PRIVATE void Interpolate();

	// ================================================================

	PRIVATE b2Body* m_body;
	PRIVATE float m_density;
	PRIVATE unsigned int m_layerIndex;

	PRIVATE bool m_interpolation;
	PRIVATE Vec2 m_backupPosition;
	PRIVATE float m_backupAngle;
};

