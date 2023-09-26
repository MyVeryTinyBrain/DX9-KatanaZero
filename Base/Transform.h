#pragma once

#include "Component.h"

class GameObject;

class Transform final : public Component
{
	COMPONENT_OVERRIDE(Awake);

	PUBLIC Mat4 LocalToWorldMatrix() const;
	PUBLIC Mat4 WorldToLocalMatrix() const;
	PUBLIC Vec2 GetUp() const;
	PUBLIC Vec2 GetRight() const;
	PUBLIC void SetUp(const Vec2& up);
	PUBLIC void SetRight(const Vec2& right);

	PUBLIC void ForceUpdateChilds(Transform* root = nullptr);
	PUBLIC Transform* GetRoot() const;
	PUBLIC Transform* GetParent() const;
	PUBLIC std::vector<Transform*> GetParents() const;
	PUBLIC void SetParent(Transform* transform);
	PUBLIC size_t GetChildCount() const;
	PUBLIC Transform* GetChild(size_t index) const;
	// 이 트랜스폼과 자식들을 모두 담은 벡터를 반환합니다.
	// 이 트랜스폼은 벡터의 0번째에 위치하게 됩니다.
	PUBLIC std::vector<Transform*> GetChilds() const;
	PUBLIC void AddChild(Transform* transform);
	PUBLIC void DetachChild(Transform* transform);
	PUBLIC void DetachAllChilds();
	PUBLIC bool GetParentEnable() const;

	PUBLIC const Vec2& GetPosition() const;
	PUBLIC const float& GetAngle() const;
	PUBLIC const Vec2& GetScale() const;
	PUBLIC const Vec2& GetLocalPosition() const;
	PUBLIC const float& GetLocalAngle() const;
	PUBLIC const Vec2& GetLocalScale() const;
	PUBLIC void SetTransform(const Vec2& position, const float& angle, const Vec2& scale);
	PUBLIC void SetPosition(const Vec2& position);
	PUBLIC void SetAngle(const float& angle);
	PUBLIC void SetScale(const Vec2& scale);
	PUBLIC void SetLocalPosition(const Vec2& localPosition);
	PUBLIC void SetLocalAngle(const float& localAngle);
	PUBLIC void SetLocalScale(const Vec2& localScale);

	PRIVATE void UpdateWorldPosition();
	PRIVATE void UpdateWorldAngle();
	PRIVATE void UpdateWorldScale();
	PRIVATE void UpdateLocalPosition();
	PRIVATE void UpdateLocalAngle();
	PRIVATE void UpdateLocalScale();
	PRIVATE void UpdateParentEnableOfThis();
	PRIVATE void UpdateParentEnableOfChilds();
	PRIVATE void ActiveParentEnableOfChilds();
	PRIVATE void DeactiveParentEnableOfChilds();

	COMPONENT_OVERRIDE(OnEnable);
	COMPONENT_OVERRIDE(OnDisable);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE Vec2 m_position;
	PRIVATE float m_angle;
	PRIVATE Vec2 m_scale;

	PRIVATE Vec2 m_localPosition;
	PRIVATE float m_localAngle;
	PRIVATE Vec2 m_localScale;

	PRIVATE bool m_parentEnable;
	PRIVATE Transform* m_parent;
	PRIVATE std::vector<Transform*> m_childs;
};

