#pragma once

#include "Component.h"
#include "GameObject.h"

template<class ComponentType>
inline ComponentType* Component::AddComponent()
{
	return GetGameObject()->AddComponent<ComponentType>();
}

template<class ComponentType>
inline bool Component::RemoveComponent()
{
	return GetGameObject()->RemoveComponent<ComponentType>();
}

inline bool Component::RemoveComponent(Component* component)
{
	return GetGameObject()->RemoveComponent(component);
}

template<class ComponentType>
inline ComponentType* Component::GetComponent()
{
	return GetGameObject()->GetComponent<ComponentType>();
}

template<class ComponentType>
inline ComponentType* Component::GetComponentInChild()
{
	return GetGameObject()->GetComponentInChild<ComponentType>();
}

template<class ComponentType>
inline ComponentType* Component::GetComponentInParent()
{
	return GetGameObject()->GetComponentInParent<ComponentType>();
}

template<class ComponentType>
inline std::vector<ComponentType*> Component::GetComponents()
{
	return GetGameObject()->GetComponents<ComponentType>();
}

template<class ComponentType>
inline std::vector<ComponentType*> Component::GetComponentsInChild()
{
	return GetGameObject()->GetComponentsInChild<ComponentType>();
}

template<class ComponentType>
inline std::vector<ComponentType*> Component::GetComponentsInParent()
{
	return GetGameObject()->GetComponentsInParent<ComponentType>();
}
