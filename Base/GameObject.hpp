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

	// �ش� �Լ��� �������̵� �� ��쿡��
	// DontUse##FunctionName() �Լ����� �ش� �Լ��� ������� �ʰ� �ϴ� ������ ���ŵ˴ϴ�.
	// ���� �ش� �Լ��� ȣ�� ��� ���Ϳ� ����� �� �ֽ��ϴ�.
	// �������̵� ���� ���� ��쿡�� DontUse##FunctionName() �Լ����� �ش� �Լ��� ������� �ʰ� �ϴ� ������ �����մϴ�.
	// ���� �ش� �Լ��� ȣ�� ��� ���Ϳ� ����� �� �����ϴ�.

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

	// ���� �� ��� Awake �Լ��� ȣ��˴ϴ�.
	component->Awake();

	// ���� �� ��� SetEnable() �Լ��� ȣ��Ǿ� Ȱ��ȭ ���·� ����˴ϴ�.
	// �̿� ���� OnEnable() �Լ��� ȣ��˴ϴ�.
	component->SetEnable(true);

	return static_cast<ComponentType*>(component);
}

template<class ComponentType>
inline bool GameObject::RemoveComponent()
{
	for (auto i = m_components.begin(); i != m_components.end(); ++i)
	{
		Component* component = *i;

		// �ı��� ������ ������Ʈ�� Ž������ �ʽ��ϴ�.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType���� ��ȯ ������ ���
		// �ش� ������Ʈ�� ���� ��û�մϴ�.
		// �� ������Ʈ�� ���� �غ� �ܰ迡�� �����ǰ� �� ������Ʈ ����Ʈ���� ���ŵ� ���Դϴ�.
		// �׸��� ������ �����̹Ƿ� ������Ʈ ����, Ž����� ���ܵ˴ϴ�.
		// ���� �� ������Ʈ�� ���� ������� �ʾҴٸ� �� ������Ʈ�� ������ �����̹Ƿ� ������� �ʽ��ϴ�.
		// �� �Լ� ��ſ� Component->Destroy() �Լ��� ȣ���ص� �̰Ͱ� ���� ������ �ϰ� �˴ϴ�.
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

		// �ı��� ������ ������Ʈ�� Ž������ �ʽ��ϴ�.
		if (currentComponent->IsShouldDestroy()) continue;

		// �� ������Ʈ�� �����Ϸ��� ������Ʈ�� ���
		// �ش� ������Ʈ�� ���� ��û�մϴ�.
		// �� ������Ʈ�� ���� �غ� �ܰ迡�� �����ǰ� �� ������Ʈ ����Ʈ���� ���ŵ� ���Դϴ�.
		// �׸��� ������ �����̹Ƿ� ������Ʈ ����, Ž����� ���ܵ˴ϴ�.
		// ���� �� ������Ʈ�� ���� ������� �ʾҴٸ� �� ������Ʈ�� ������ �����̹Ƿ� ������� �ʽ��ϴ�.
		// �� �Լ� ��ſ� Component->Destroy() �Լ��� ȣ���ص� �̰Ͱ� ���� ������ �ϰ� �˴ϴ�.
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

		// �ı��� ������ ������Ʈ�� Ž������ �ʽ��ϴ�.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType���� ��ȯ ������ ���
		// �� ������Ʈ�� ��ȯ�մϴ�.
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
	// �� Ʈ�������� ��� �ڽĵ��� ����ִ� �����Դϴ�.
	std::vector<Transform*> childs = GetTransform()->GetChilds();

	// �� Ʈ�������� ��� �ڽĵ��� ��ȸ�ϸ�
	// Ÿ�԰� ��ġ�ϴ� ������Ʈ�� �ִٸ� �ش� ������Ʈ�� ��ȯ�մϴ�.
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
	// �� Ʈ�������� ��� �θ���� ����ִ� �����Դϴ�.
	std::vector<Transform*> parents = GetTransform()->GetParents();

	// �� Ʈ�������� ��� �ڽĵ��� ��ȸ�ϸ�
	// Ÿ�԰� ��ġ�ϴ� ������Ʈ�� �ִٸ� �ش� ������Ʈ�� ��ȯ�մϴ�.
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

		// �ı��� ������ ������Ʈ�� Ž������ �ʽ��ϴ�.
		if (component->IsShouldDestroy()) continue;

		ComponentType* cast = dynamic_cast<ComponentType*>(component);

		// ComponentType���� ��ȯ ������ ���
		// �� ������Ʈ�� ���Ϳ� �����մϴ�.
		if (cast)
		{
			v.push_back(cast);
		}
	}

	// ���� Ŭ�������� �̵� �����ڰ� �����Ǿ� �ֽ��ϴ�.
	// ��ȯ �� �̵� �����ڰ� ȣ��Ǿ� ���� ���� ���������� ��ȯ�˴ϴ�.
	return v;
}

template<class ComponentType>
inline std::vector<ComponentType*> GameObject::GetComponentsInChild()
{
	// �� Ʈ�������� ��� �ڽĵ��� ����ִ� �����Դϴ�.
	std::vector<Transform*> childs = GetTransform()->GetChilds();

	// �� Ʈ�������� �ڽĵ��� ��� �ش� ������Ʈ�� ��ġ�ϴ� ������Ʈ���� ���� �� �����Դϴ�.
	std::vector<ComponentType*> v;

	// �� Ʈ�������� ��� �ڽĵ��� ��ȸ�ϸ�
	// Ÿ�԰� ��ġ�ϴ� ������Ʈ�� ���Ϳ� �����մϴ�.
	for (auto& transform : childs)
	{
		// �� Ʈ�������� ���ӿ�����Ʈ�� �پ��ִ� ��ġ�ϴ� ��� ������Ʈ���� ����� �����Դϴ�.
		std::vector<ComponentType*> components = transform->GetGameObject()->GetComponents<ComponentType>();

		// ���� ������ ������Ʈ���� v ���Ϳ� �����մϴ�.
		for (auto& component : components)
		{
			v.push_back(component);
		}
	}

	// ���� Ŭ�������� �̵� �����ڰ� �����Ǿ� �ֽ��ϴ�.
	// ��ȯ �� �̵� �����ڰ� ȣ��Ǿ� ���� ���� ���������� ��ȯ�˴ϴ�.
	return v;
}

template<class ComponentType>
inline std::vector<ComponentType*> GameObject::GetComponentsInParent()
{
	// �� Ʈ�������� ��� �θ���� ����ִ� �����Դϴ�.
	std::vector<Transform*> parents = GetTransform()->GetParents();

	// �� Ʈ�������� �θ���� ��� �ش� ������Ʈ�� ��ġ�ϴ� ������Ʈ���� ���� �� �����Դϴ�.
	std::vector<ComponentType*> v;

	// �� Ʈ�������� ��� �ڽĵ��� ��ȸ�ϸ�
	// Ÿ�԰� ��ġ�ϴ� ������Ʈ�� ���Ϳ� �����մϴ�.
	for (auto& transform : parents)
	{
		// �� Ʈ�������� ���ӿ�����Ʈ�� �پ��ִ� ��ġ�ϴ� ��� ������Ʈ���� ����� �����Դϴ�.
		std::vector<ComponentType*> components = transform->GetGameObject()->GetComponents<ComponentType>();

		// ���� ������ ������Ʈ���� v ���Ϳ� �����մϴ�.
		for (auto& component : components)
		{
			v.push_back(component);
		}
	}

	// ���� Ŭ�������� �̵� �����ڰ� �����Ǿ� �ֽ��ϴ�.
	// ��ȯ �� �̵� �����ڰ� ȣ��Ǿ� ���� ���� ���������� ��ȯ�˴ϴ�.
	return v;
}
