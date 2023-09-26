#pragma once

#include "Object.h"

class Component;
class Transform;

class GameObject final : public Object
{
	friend class Transform;
	friend class Job;

	PUBLIC GameObject(Transform* parent = nullptr);
	PUBLIC GameObject(const Vec2& position, Transform* parent = nullptr);
	PUBLIC GameObject(const Vec2& posiition, float angle, Transform* parent = nullptr);
	PUBLIC GameObject(const Vec2& position, float angle, const Vec2& scale, Transform* parent = nullptr);
	PUBLIC static GameObject* CreateChild(GameObject* parent);

	// ���ӿ�����Ʈ�� ������ Destroy() �Լ��� ����մϴ�.
	PRIVATE virtual ~GameObject();

	// ���� RemoveComponent() �Լ��� Transform ������Ʈ�� �����ϴ� ���
	// �� ���ӿ�����Ʈ�� Destroy()�� ȣ���� �Ͱ� �����ϴ�.
	// ���� �� ���ӿ�����Ʈ�� ��� �ڽĵ��� ���ŵ˴ϴ�.

	PUBLIC template <class ComponentType> ComponentType* AddComponent();
	PUBLIC template <class ComponentType> bool RemoveComponent();
	PUBLIC bool RemoveComponent(Component* component);
	PUBLIC template <class ComponentType> ComponentType* GetComponent();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInChild();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInParent();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponents();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInChild();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInParent();

	// �� ���ӿ�����Ʈ�� Ȱ��ȭ �����Դϴ�.
	PUBLIC bool GetActive() const;

	// GetEnable() �Լ��� �ܺο��� ����� �� �����ϴ�.
	// ��� GetActive() �Լ��� ����մϴ�.
	// ���� �� �Լ��� ����Ϸ��� Object Ÿ������ �ٿ�ĳ�����Ͽ� ����ϼ���.
	PRIVATE bool GetEnable() const;

	PUBLIC Transform* GetTransform() const;
	PUBLIC const wstring& GetName() const;
	PUBLIC tag_t GetTag() const;
	PUBLIC void SetName(const wstring& name);
	PUBLIC void SetTag(tag_t tag);

	PRIVATE void CallOnEnabledGameObject();
	PRIVATE void CallOnDisabledGameObject();
	PRIVATE void OnActiveChanged();
	PRIVATE void OnEnabledParent();
	PRIVATE void OnDisabledParent();

	COMPONENT_OVERRIDE(OnEnable);
	COMPONENT_OVERRIDE(OnDisable);
	COMPONENT_OVERRIDE(OnDestroy);

	// ���ӿ�����Ʈ�� �����Ǿ��� �� Ʈ������ ������Ʈ�� ������ 0��°�� �߰��ϴ� �Լ��Դϴ�.
	PRIVATE Transform* AddTransform();

	PRIVATE wstring m_name;
	PRIVATE tag_t m_tag;

	PRIVATE bool m_prevActive;

	// �� ���ӿ�����Ʈ�� ������ ��� ������Ʈ���Դϴ�.
	PRIVATE std::vector<Component*> m_components;

	// ===================================================================================================

	// ��� ���ӿ�����Ʈ�� ���ŵǵ��� �մϴ�.
	PUBLIC static void DestroyAll();

	// ������ ��� ���ӿ�����Ʈ�� �� ���� �ȿ� ���ϴ�.
	PRIVATE static std::vector<GameObject*> g_gameObjects;
};

#include "GameObject.hpp"
