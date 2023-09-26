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
	// �����Ǹ� Ȱ��ȭ ���·� ���۵˴ϴ�.
	m_enable = true;

	// Ʈ������ ������Ʈ�� �����ϰ� �Ű������� �˸°� �����մϴ�.
	Transform* transform = AddTransform();
	transform->SetPosition(position);
	transform->SetAngle(angle);
	transform->SetScale(scale);
	transform->SetParent(parent);

	// �ٸ� �������� �ʱ�ȭ�մϴ�.
	m_name = L"";
	m_tag = 0;

	m_prevActive = GetActive();

	// ���ӿ�����Ʈ ���Ϳ� �� ���ӿ�����Ʈ�� �߰��մϴ�.
	g_gameObjects.push_back(this);
}

GameObject* GameObject::CreateChild(GameObject* parent)
{
	Transform* pt = parent->GetTransform();
	return new GameObject(pt->GetPosition(), pt->GetAngle(), pt->GetScale(), pt);
}

GameObject::~GameObject()
{
	// ������ ��� ������Ʈ�� Destroy() �Լ��� ȣ���մϴ�.
	// �̿� ���� �� ������Ʈ�� OnDisable(), OnDestroy() �Լ��� ȣ��˴ϴ�.
	for (auto& component : m_components)
		component->Destroy();

	// ��� �̺�Ʈ �Լ��� ȣ���� ��
	// �� ������Ʈ���� �Ҹ��ŵ�ϴ�.
	for (auto& component : m_components)
		SAFE_DELETE(component);

	// ������Ʈ ����Ʈ�� ���ϴ�.
	m_components.clear();
}

bool GameObject::GetActive() const
{
	// �� ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ��ְ� �θ� ���� Ȱ��ȭ�϶� True�� ��ȯ�մϴ�.
	return GetEnable() && GetTransform()->GetParentEnable();
}

bool GameObject::GetEnable() const
{
	return Object::GetEnable();
}

Transform* GameObject::GetTransform() const
{
	// 0��° ���Ұ� ���ٸ� nullptr�� ��ȯ�մϴ�.
	if (m_components.size() == 0) return nullptr;

	// ���ӿ�����Ʈ�� ������Ʈ ���� 0��° ������Ʈ�� ������ Ʈ�������Դϴ�.
	// �� 0��° ���Ҹ� ��ȯ�մϴ�.
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
	// �� ���ӿ�����Ʈ�� ��Ȱ��ȭ �Ǿ��� ��
	// �� ���ӿ�����Ʈ�� Ʈ������ ���� ��Ȱ��ȭ���� Ʈ���� Ȱ��ȭ ���¸� �����մϴ�.
	TRANSFORM->SetEnable(true);

	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnDisable()
{	
	// �� ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ��� ��
	// �� ���ӿ�����Ʈ�� Ʈ������ ���� Ȱ��ȭ���� Ʈ���� Ȱ��ȭ ���¸� �����մϴ�.
	TRANSFORM->SetEnable(false);

	if (m_prevActive != GetActive())
		OnActiveChanged();
}

void GameObject::OnDestroy()
{
	// �� ���ӿ�����Ʈ�� �����Ƿ��� �� ��
	// �� ���ӿ�����Ʈ�� Ʈ������ ���� ���� ��û�Ͽ� Ʈ���� ��� ������Ʈ�� ���ӿ�����Ʈ���� ���� ��û�մϴ�.
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
