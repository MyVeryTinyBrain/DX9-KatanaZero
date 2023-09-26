#pragma once

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

template<class ComponentType>
inline ComponentType* GameObject::AddComponent()
{
	Component* component = new ComponentType();
	m_components.push_back(component);
	component->m_transform = GetTransform();
	component->m_gameObject = this;

	// 해당 함수가 오버라이드 된 경우에는
	// DontUse##FunctionName() 함수에서 해당 함수를 사용하지 않게 하는 내용이 제거됩니다.
	// 따라서 해당 함수가 호출 대상 벡터에 저장될 수 있습니다.
	// 오버라이드 되지 않은 경우에는 DontUse##FunctionName() 함수에서 해당 함수를 사용하지 않게 하는 내용이 존재합니다.
	// 따라서 해당 함수가 호출 대상 벡터에 저장될 수 없습니다.

	component->DontUseStart();
	component->DontUseBeforeFixedStep();
	component->DontUseFixedUpdate();
	component->DontUseLateFixedUpdate();
	component->DontUseUpdate();
	component->DontUseLateUpdate();
	component->DontUseAfterUpdate();
	component->DontUseBeforeRender();
	component->DontUseRender();
	component->DontUseAfterRender();

	// 생성 후 즉시 Awake 함수가 호출됩니다.
	component->Awake();

	// 생성 후 즉시 SetEnable() 함수가 호출되어 활성화 상태로 변경됩니다.
	// 이에 따라 OnEnable() 함수가 호출됩니다.
	component->SetEnable(true);

	return static_cast<ComponentType*>(component);
}

template<class ComponentType>
inline bool GameObject::RemoveComponent()
{
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
	{
		Component* component = *i;

		// 파괴될 예정인 컴포넌트는 탐색하지 않습니다.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType으로 변환 가능한 경우
		// 해당 컴포넌트를 삭제 요청합니다.
		// 이 컴포넌트는 다음 준비 단계에서 삭제되고 이 컴포넌트 리스트에서 제거될 것입니다.
		// 그리고 삭제될 예정이므로 컴포넌트 삭제, 탐색등에서 제외됩니다.
		// 또한 이 컴포넌트가 아직 실행되지 않았다면 이 컴포넌트는 삭제될 예정이므로 실행되지 않습니다.
		// 이 함수 대신에 Component->Destroy() 함수를 호출해도 이것과 같은 동작을 하게 됩니다.
		if (cast)
		{
			component->Destroy();
			return true;
		}
	}
	return false;
}

inline bool GameObject::RemoveComponent(Component* component)
{
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
	{
		Component* currentComponent = *i;

		// 파괴될 예정인 컴포넌트는 탐색하지 않습니다.
		if (currentComponent->IsShouldDestroy()) continue;

		// 이 컴포넌트가 삭제하려는 컴포넌트인 경우
		// 해당 컴포넌트를 삭제 요청합니다.
		// 이 컴포넌트는 다음 준비 단계에서 삭제되고 이 컴포넌트 리스트에서 제거될 것입니다.
		// 그리고 삭제될 예정이므로 컴포넌트 삭제, 탐색등에서 제외됩니다.
		// 또한 이 컴포넌트가 아직 실행되지 않았다면 이 컴포넌트는 삭제될 예정이므로 실행되지 않습니다.
		// 이 함수 대신에 Component->Destroy() 함수를 호출해도 이것과 같은 동작을 하게 됩니다.
		if (currentComponent == component)
		{
			currentComponent->Destroy();
			return true;
		}
	}
	return false;
}

template<class ComponentType>
inline ComponentType* GameObject::GetComponent()
{
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
	{
		Component* component = *i;

		// 파괴될 예정인 컴포넌트는 탐색하지 않습니다.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType으로 변환 가능한 경우
		// 이 컴포넌트를 반환합니다.
		if (cast)
		{
			return cast;
		}
	}
	return nullptr;
}

template<class ComponentType>
inline ComponentType* GameObject::GetComponentInChild()
{
	// 이 트랜스폼과 모든 자식들이 담겨있는 벡터입니다.
	std::vector<Transform*> childs = GetTransform()->GetChilds();

	// 이 트랜스폼과 모든 자식들을 순회하며
	// 타입과 일치하는 컴포넌트가 있다면 해당 컴포넌트를 반환합니다.
	for (auto& transform : childs)
	{
		ComponentType* component = transform->GetGameObject()->GetComponent<ComponentType>();
		if (component)
		{
			return component;
		}
	}

	return nullptr;
}

template<class ComponentType>
inline ComponentType* GameObject::GetComponentInParent()
{
	// 이 트랜스폼과 모든 부모들이 담겨있는 벡터입니다.
	std::vector<Transform*> parents = GetTransform()->GetParents();

	// 이 트랜스폼과 모든 자식들을 순회하며
	// 타입과 일치하는 컴포넌트가 있다면 해당 컴포넌트를 반환합니다.
	for (auto& transform : parents)
	{
		ComponentType* component = transform->GetGameObject()->GetComponent<ComponentType>();
		if (component)
		{
			return component;
		}
	}

	return nullptr;
}

template<class ComponentType>
inline std::vector<ComponentType*> GameObject::GetComponents()
{
	std::vector<ComponentType*> v;

	for (auto i = m_components.begin(); i != m_components.end(); ++i)
	{
		Component* component = *i;

		// 파괴될 예정인 컴포넌트는 탐색하지 않습니다.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType으로 변환 가능한 경우
		// 이 컴포넌트를 벡터에 저장합니다.
		if (cast)
		{
			v.push_back(cast);
		}
	}

	// 벡터 클래스에는 이동 연산자가 구현되어 있습니다.
	// 반환 시 이동 연산자가 호출되어 복사 없이 정상적으로 반환됩니다.
	return v;
}

template<class ComponentType>
inline std::vector<ComponentType*> GameObject::GetComponentsInChild()
{
	// 이 트랜스폼과 모든 자식들이 담겨있는 벡터입니다.
	std::vector<Transform*> childs = GetTransform()->GetChilds();

	// 이 트랜스폼과 자식들의 모든 해당 컴포넌트와 일치하는 컴포넌트들이 담기게 될 벡터입니다.
	std::vector<ComponentType*> v;

	// 이 트랜스폼과 모든 자식들을 순회하며
	// 타입과 일치하는 컴포넌트를 벡터에 저장합니다.
	for (auto& transform : childs)
	{
		// 이 트랜스폼의 게임오브젝트에 붙어있는 일치하는 모든 컴포넌트들이 저장된 벡터입니다.
		std::vector<ComponentType*> components = transform->GetGameObject()->GetComponents<ComponentType>();

		// 위의 벡터의 컴포넌트들을 v 벡터에 저장합니다.
		for (auto& component : components)
		{
			v.push_back(component);
		}
	}

	// 벡터 클래스에는 이동 연산자가 구현되어 있습니다.
	// 반환 시 이동 연산자가 호출되어 복사 없이 정상적으로 반환됩니다.
	return v;
}

template<class ComponentType>
inline std::vector<ComponentType*> GameObject::GetComponentsInParent()
{
	// 이 트랜스폼과 모든 부모들이 담겨있는 벡터입니다.
	std::vector<Transform*> parents = GetTransform()->GetParents();

	// 이 트랜스폼과 부모들의 모든 해당 컴포넌트와 일치하는 컴포넌트들이 담기게 될 벡터입니다.
	std::vector<ComponentType*> v;

	// 이 트랜스폼과 모든 자식들을 순회하며
	// 타입과 일치하는 컴포넌트를 벡터에 저장합니다.
	for (auto& transform : parents)
	{
		// 이 트랜스폼의 게임오브젝트에 붙어있는 일치하는 모든 컴포넌트들이 저장된 벡터입니다.
		std::vector<ComponentType*> components = transform->GetGameObject()->GetComponents<ComponentType>();

		// 위의 벡터의 컴포넌트들을 v 벡터에 저장합니다.
		for (auto& component : components)
		{
			v.push_back(component);
		}
	}

	// 벡터 클래스에는 이동 연산자가 구현되어 있습니다.
	// 반환 시 이동 연산자가 호출되어 복사 없이 정상적으로 반환됩니다.
	return v;
}
