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

	// 게임오브젝트의 삭제는 Destroy() 함수를 사용합니다.
	PRIVATE virtual ~GameObject();

	// 만약 RemoveComponent() 함수로 Transform 컴포넌트를 제거하는 경우
	// 이 게임오브젝트에 Destroy()를 호출한 것과 같습니다.
	// 따라서 이 게임오브젝트와 모든 자식들이 제거됩니다.

	PUBLIC template <class ComponentType> ComponentType* AddComponent();
	PUBLIC template <class ComponentType> bool RemoveComponent();
	PUBLIC bool RemoveComponent(Component* component);
	PUBLIC template <class ComponentType> ComponentType* GetComponent();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInChild();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInParent();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponents();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInChild();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInParent();

	// 이 게임오브젝트의 활성화 상태입니다.
	PUBLIC bool GetActive() const;

	// GetEnable() 함수는 외부에서 사용할 수 없습니다.
	// 대신 GetActive() 함수를 사용합니다.
	// 만약 이 함수를 사용하려면 Object 타입으로 다운캐스팅하여 사용하세요.
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

	// 게임오브젝트가 생성되었을 때 트랜스폼 컴포넌트를 벡터의 0번째에 추가하는 함수입니다.
	PRIVATE Transform* AddTransform();

	PRIVATE wstring m_name;
	PRIVATE tag_t m_tag;

	PRIVATE bool m_prevActive;

	// 이 게임오브젝트에 부착된 모든 컴포넌트들입니다.
	PRIVATE std::vector<Component*> m_components;

	// ===================================================================================================

	// 모든 게임오브젝트가 제거되도록 합니다.
	PUBLIC static void DestroyAll();

	// 생성된 모든 게임오브젝트는 이 벡터 안에 담깁니다.
	PRIVATE static std::vector<GameObject*> g_gameObjects;
};

#include "GameObject.hpp"
