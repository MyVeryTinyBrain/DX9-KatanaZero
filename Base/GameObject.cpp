#include "base_stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

#define TRANSFORM (GetTransform())

std::vector<GameObject*> GameObject::g_gameObjects;

GameObject::GameObject(Transform* parent) : GameObject({}, 0, { 1, 1 }, parent)
{
}

GameObject::GameObject(const Vec2& position, Transform* parent) : GameObject(position, 0, { 1, 1 }, parent)
{
}

GameObject::GameObject(const Vec2& posiition, float angle, Transform* parent) : GameObject(posiition, angle, { 1, 1 }, parent)
{
}

GameObject::GameObject(const Vec2& position, float angle, const Vec2& scale, Transform* parent)
{
	// 생성되면 활성화 상태로 시작됩니다.
	m_enable = true;

	// 트랜스폼 컴포넌트를 부착하고 매개변수에 알맞게 설정합니다.
	Transform* transform = AddTransform();
	transform->SetPosition(position);
	transform->SetAngle(angle);
	transform->SetScale(scale);
	transform->SetParent(parent);

	// 다른 변수들을 초기화합니다.
	m_name = L"";
	m_tag = 0;

	m_prevActive = GetActive();

	// 게임오브젝트 벡터에 이 게임오브젝트를 추가합니다.
	g_gameObjects.push_back(this);
}

GameObject* GameObject::CreateChild(GameObject* parent)
{
	Transform* pt = parent->GetTransform();
	return new GameObject(pt->GetPosition(), pt->GetAngle(), pt->GetScale(), pt);
}

GameObject::~GameObject()
{
	// 부착된 모든 컴포넌트의 Destroy() 함수를 호출합니다.
	// 이에 따라서 각 컴포넌트의 OnDisable(), OnDestroy() 함수가 호출됩니다.
	for (auto& component : m_components)
		component->Destroy();

	// 모든 이벤트 함수를 호출한 후
	// 각 컴포넌트들을 소멸시킵니다.
	for (auto& component : m_components)
		SAFE_DELETE(component);

	// 컴포넌트 리스트를 비웁니다.
	m_components.clear();
}

bool GameObject::GetActive() const
{
	// 이 게임오브젝트가 활성화 되어있고 부모 또한 활성화일때 True를 반환합니다.
	return GetEnable() && GetTransform()->GetParentEnable();
}

bool GameObject::GetEnable() const
{
	return Object::GetEnable();
}

Transform* GameObject::GetTransform() const
{
	// 0번째 원소가 없다면 nullptr을 반환합니다.
	if (m_components.size() == 0) return nullptr;

	// 게임오브젝트의 컴포넌트 벡터 0번째 컴포넌트는 언제나 트랜스폼입니다.
	// 이 0번째 원소를 반환합니다.
	return static_cast<Transform*>(m_components[0]);
}

const wstring& GameObject::GetName() const
{
	return m_name;
}

tag_t GameObject::GetTag() const
{
	return m_tag;
}

void GameObject::SetName(const wstring& name)
{
	m_name = name;
}

void GameObject::SetTag(tag_t tag)
{
	m_tag = tag;
}

void GameObject::CallOnEnabledGameObject()
{
	for (auto& component : m_components)
	{
		component->OnEnabledGameObject();
	}
}

void GameObject::CallOnDisabledGameObject()
{
	for (auto& component : m_components)
	{
		component->OnDisabledGameObject();
	}
}

void GameObject::OnActiveChanged()
{
	bool currentActive = GetActive();
	m_prevActive = currentActive;

	if (currentActive == true) CallOnEnabledGameObject();
	else CallOnDisabledGameObject();
}

void GameObject::OnEnabledParent()
{
	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnDisabledParent()
{
	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnEnable()
{
	// 이 게임오브젝트가 비활성화 되었을 때
	// 이 게임오브젝트의 트랜스폼 또한 비활성화시켜 트리의 활성화 상태를 갱신합니다.
	TRANSFORM->SetEnable(true);

	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnDisable()
{	
	// 이 게임오브젝트가 활성화 되었을 때
	// 이 게임오브젝트의 트랜스폼 또한 활성화시켜 트리의 활성화 상태를 갱신합니다.
	TRANSFORM->SetEnable(false);

	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnDestroy()
{
	// 이 게임오브젝트가 삭제되려고 할 때
	// 이 게임오브젝트의 트랜스폼 또한 삭제 요청하여 트리의 모든 컴포넌트와 게임오브젝트들을 삭제 요청합니다.
	TRANSFORM->Destroy();
}

Transform* GameObject::AddTransform()
{
	Transform* transform = AddComponent<Transform>();
	Component* component = transform;
	component->m_transform = transform;
	component->m_gameObject = this;
	return transform;
}

void GameObject::DestroyAll()
{
	for (auto& gameObject : g_gameObjects)
	{
		gameObject->Destroy();
	}
}
