#include "base_stdafx.h"
#include "Transform.h"
#include "GameObject.h"

#define GAMEOBJECT (GetGameObject())

void Transform::Awake()
{
	m_position = {};
	m_angle = {};
	m_scale = {};
	m_localPosition = {};
	m_localAngle = {};
	m_localScale = {};
	m_parentEnable = true;
	m_parent = nullptr;
	//m_childs.clear();
}

Mat4 Transform::LocalToWorldMatrix() const
{
	return Mat4::SRT({ m_scale.x, m_scale.y, 1.0f }, { 0, 0, m_angle * DEG2RAD }, { m_position.x, m_position.y, 0.5f });
}

Mat4 Transform::WorldToLocalMatrix() const
{
	return Mat4::TRS({ -m_position.x, -m_position.y, 0.5f }, { 0, 0, -m_angle * DEG2RAD }, {1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f});
}

Vec2 Transform::GetUp() const
{
	// y���� ��Ÿ���� ��ȯ�� ǥ�� ������ ��ȯ�մϴ�.
	return LocalToWorldMatrix().MultiplyVector(Vec2::up());
}

Vec2 Transform::GetRight() const
{
	// x���� ��Ÿ���� ��ȯ�� ǥ�� ������ ��ȯ�մϴ�.
	return LocalToWorldMatrix().MultiplyVector(Vec2::right());
}

void Transform::SetUp(const Vec2& up)
{
	// y���� ��Ÿ���� ǥ�� ������ �Ű����� up ������ �ٶ󺸵��� ������ �����մϴ�.
	Vec2 yBasis = up.normalized();
	float angle = Quat::FromToRotation(Vec2::up(), yBasis).GetEulerAngle().z;
	SetAngle(angle);
}

void Transform::SetRight(const Vec2& right)
{
	// x���� ��Ÿ���� ǥ�� ������ �Ű����� right ������ �ٶ󺸵��� ������ �����մϴ�.
	Vec2 xBasis = right.normalized();
	float angle = Quat::FromToRotation(Vec2::right(), xBasis).GetEulerAngle().z;
	SetAngle(angle);
}

void Transform::ForceUpdateChilds(Transform* root)
{
	// �ڽ� ��ü���� ���� Ʈ�������� �����մϴ�.

	// root�� nullptr�� ���
	// �� Ʈ�������� �ֻ�� �θ� root�� �����մϴ�.
	if (root == nullptr) root = GetRoot();

	// �ڽĵ��� ���� �� �����Դϴ�.
	std::stack<Transform*> stack;

	stack.push(root);

	// root�� ��� �ڽĵ��� ���� Ʈ�������� �����մϴ�.
	// root�� ����/���� Ʈ�������� ����� ��쿡
	// �ٲ��� �� ���� ���� Ʈ������ ���Դϴ�.
	// ���� ���� Ʈ���������� �����մϴ�.
	while (!stack.empty())
	{
		Transform* transform = stack.top();
		stack.pop();

		for (auto& child : transform->m_childs)
		{
			child->UpdateWorldPosition();
			child->UpdateWorldAngle();
			child->UpdateWorldScale();
			stack.push(child);
		}
	}
}

Transform* Transform::GetRoot() const
{
	const Transform* transform = this;
	while (transform->m_parent)
		transform = transform->m_parent;
	return const_cast<Transform*>(transform);
}

Transform* Transform::GetParent() const
{
	return m_parent;
}

std::vector<Transform*> Transform::GetParents() const
{
	std::vector<Transform*> v;

	Transform* current = const_cast<Transform*>(this);
	while (current)
	{
		v.push_back(current);
		current = current->m_parent;
	}

	return v;
}

void Transform::SetParent(Transform* transform)
{
	// ���� �θ�� �����Ϸ��� ��� ������ ���̱� ���� �Լ��� �����մϴ�.
	if (transform == m_parent) return;

	// �̹� �θ� �ִ� ��쿡
	// �� �θ𿡰Լ� Ż���մϴ�.
	if (m_parent)
		m_parent->DetachChild(this);

	// �����Ϸ��� �θ� nullptr�� �ƴ� ���
	// �� Ʈ�������� �ڽ����� �߰��մϴ�.
	// AddChild �Լ����� �� Ʈ�������� m_parent�� �����մϴ�.
	if (transform)
		transform->AddChild(this);
	// �����Ϸ��� �θ� nullptr�� ���
	// m_parent�� nullptr�� �����մϴ�.
	else
		m_parent = nullptr;
}

size_t Transform::GetChildCount() const
{
	return m_childs.size();
}

Transform* Transform::GetChild(size_t index) const
{
	return m_childs[index];
}

std::vector<Transform*> Transform::GetChilds() const
{
	// �� Ʈ�������� �����ϰ�
	// ���� ��� �ڽĵ��� ��� �����ϴ� �����Դϴ�.
	// �ܼ��� �� Ʈ�������� �ڽĵ��� �ƴ� �ڽĵ��� �ڽĵ���� ��� ����ϴ�.
	std::vector<Transform*> v;

	std::stack<const Transform*> stack;
	stack.push(this);

	while (!stack.empty())
	{
		const Transform* transform = stack.top();
		stack.pop();

		// �ڽ� Ʈ���������� ���ÿ� ����ϴ�.
		for (auto& child : transform->m_childs)
			stack.push(child);

		// Ʈ�������� ���Ϳ� ����ϴ�.
		v.push_back(const_cast<Transform*>(transform));
	}

	return v;
}

void Transform::AddChild(Transform* transform)
{
	transform->m_parent = this;
	transform->UpdateLocalPosition();
	transform->UpdateLocalAngle();
	transform->UpdateLocalScale();
	transform->UpdateParentEnableOfThis();
	transform->UpdateParentEnableOfChilds();
	m_childs.push_back(transform);
}

void Transform::DetachChild(Transform* transform)
{
	// �����Ϸ��� Ʈ�������� nullptr�� ��� �Լ��� �����մϴ�.
	if (transform == nullptr) return;

	// �Ű������� Ʈ�������� �ڽ��� �ƴ϶�� �Լ��� �����մϴ�.
	auto contains = std::find(m_childs.begin(), m_childs.end(), transform);
	if (contains == m_childs.end()) return;

	transform->m_parent = nullptr;
	transform->UpdateLocalPosition();
	transform->UpdateLocalAngle();
	transform->UpdateLocalScale();
	transform->UpdateParentEnableOfThis();
	transform->UpdateParentEnableOfChilds();

	m_childs.erase(contains);
}

void Transform::DetachAllChilds()
{
	for (auto& child : m_childs)
	{
		child->m_parent = nullptr;
		child->UpdateLocalPosition();
		child->UpdateLocalAngle();
		child->UpdateLocalScale();
		child->UpdateParentEnableOfThis();
	}
	m_childs.clear();
}

bool Transform::GetParentEnable() const
{
	return m_parentEnable;
}

const Vec2& Transform::GetPosition() const
{
	return m_position;
}

const float& Transform::GetAngle() const
{
	return m_angle;
}

const Vec2& Transform::GetScale() const
{
	return m_scale;
}

const Vec2& Transform::GetLocalPosition() const
{
	return m_localPosition;
}

const float& Transform::GetLocalAngle() const
{
	return m_localAngle;
}

const Vec2& Transform::GetLocalScale() const
{
	return m_localScale;
}

void Transform::SetTransform(const Vec2& position, const float& angle, const Vec2& scale)
{
	m_position = position;
	m_angle = angle;
	m_scale = scale;
	UpdateLocalPosition();
	UpdateLocalAngle();
	UpdateLocalScale();
	ForceUpdateChilds();
}

void Transform::SetPosition(const Vec2& position)
{
	m_position = position;
	UpdateLocalPosition();
	ForceUpdateChilds();
}

void Transform::SetAngle(const float& angle)
{
	m_angle = angle;
	UpdateLocalAngle();
	ForceUpdateChilds();
}

void Transform::SetScale(const Vec2& scale)
{
	m_scale = scale;
	UpdateLocalScale();
	ForceUpdateChilds();
}

void Transform::SetLocalPosition(const Vec2& localPosition)
{
	m_localPosition = localPosition;
	UpdateWorldPosition();
	ForceUpdateChilds();
}

void Transform::SetLocalAngle(const float& localAngle)
{
	m_localAngle = localAngle;
	UpdateWorldAngle();
	ForceUpdateChilds();
}

void Transform::SetLocalScale(const Vec2& localScale)
{
	m_localScale = localScale;
	UpdateWorldScale();
	ForceUpdateChilds();
}

void Transform::UpdateWorldPosition()
{
	if (m_parent)
		m_position = Vec4(m_localPosition.x, m_localPosition.y, 0, 1) * m_parent->LocalToWorldMatrix();
	else
		m_position = m_localPosition;
}

void Transform::UpdateWorldAngle()
{
	if (m_parent)
		m_angle = m_localAngle + m_parent->m_angle;
	else
		m_angle = m_localAngle;
}

void Transform::UpdateWorldScale()
{
	if (m_parent)
		m_scale = { m_localScale.x * m_parent->m_scale.x, m_localScale.y * m_parent->m_scale.y };
	else
		m_scale = m_localScale;
}

void Transform::UpdateLocalPosition()
{
	if (m_parent)
		m_localPosition = Vec4(m_position.x, m_position.y, 0, 1) * m_parent->WorldToLocalMatrix();
	else
		m_localPosition = m_position;
}

void Transform::UpdateLocalAngle()
{
	if (m_parent)
		m_localAngle = m_angle - m_parent->m_angle;
	else
		m_localAngle = m_angle;
}

void Transform::UpdateLocalScale()
{
	if (m_parent)
		m_localScale = { m_scale.x / m_parent->m_scale.x, m_scale.y / m_parent->m_scale.y };
	else
		m_localScale = m_scale;
}

void Transform::UpdateParentEnableOfThis()
{
	bool prev = m_parentEnable;

	// ���� Ȱ��ȭ ���¸� �����մϴ�.
	// ���� Ȱ��ȭ ���¸� ������ �θ� ��Ȱ��ȭ ������ ��쿡 �ڽĵ� ��Ȱ��ȭ ���·� �ν��� �� �ֽ��ϴ�.
	// ���� Ȱ��ȭ ���¿� ���� Ȱ��ȭ ���°� ��� Ȱ��ȭ �Ǿ������� Ȱ��ȭ ���·� �ν��ϵ��� �մϴ�.

	if (m_parent)
	{
		// �θ� �ִ� ���
		// �θ� ��Ȱ��ȭ �����̰ų�, �θ��� �θ� ��Ȱ��ȭ �����ΰ�� ��Ȱ��ȭ�� �����մϴ�.
		// �θ� Ȱ��ȭ �����̰�, �θ��� �θ� ���� Ȱ��ȭ �����̸� Ȱ��ȭ�� �����մϴ�.
		m_parentEnable = m_parent->GetEnable() && m_parent->GetParentEnable();
	}
	else
	{
		// �θ� ���� ���
		// Ȱ��ȭ�� �����մϴ�.
		m_parentEnable = true;
	}

	if (prev != m_parentEnable)
	{
		if (m_parentEnable) GetGameObject()->OnEnabledParent();
		else GetGameObject()->OnDisabledParent();
	}
}

void Transform::UpdateParentEnableOfChilds()
{
	if (m_parentEnable == true) ActiveParentEnableOfChilds();
	else DeactiveParentEnableOfChilds();
}

void Transform::ActiveParentEnableOfChilds()
{
	// �ڽĵ��� ���� �� �����Դϴ�.
	std::stack<Transform*> stack;
	stack.push(this);

	while (!stack.empty())
	{
		Transform* transform = stack.top();
		stack.pop();

		// �ش� Ʈ�������� ��Ȱ��ȭ ���¶�� ���� Ȱ��ȭ ���¸� �ٲ��� �ʽ��ϴ�.
		// �׸��� �� Ʈ�������� �ڽĵ��� ���ÿ� ���� �ʰ� ���� ������ ���ҷ� �Ѿ�ϴ�.
		// ������ ���� Ȱ��ȭ ���°� ��Ȱ��ȭ �����̹Ƿ� �� Ʈ�������� �ڽĵ� ���� ���� ��Ȱ��ȭ ���´� ��Ȱ��ȭ ������ ���Դϴ�.
		if (transform->GetEnable() == false) continue;

		bool prev = transform->m_parentEnable;

		// �ش� Ʈ�������� Ȱ��ȭ �����̰� �θ� ���� Ȱ��ȭ ���¶�� ���� Ȱ��ȭ ���� Ȱ��ȭ ���·� �ٲ�ϴ�.
		transform->m_parentEnable = true;

		if (prev != transform->m_parentEnable)
		{
			if (m_parentEnable) transform->GetGameObject()->OnEnabledParent();
			else transform->GetGameObject()->OnDisabledParent();
		}

		// �ڽ� Ʈ���������� ���ÿ� ����ϴ�.
		for (auto& child : transform->m_childs)
			stack.push(child);
	}
}

void Transform::DeactiveParentEnableOfChilds()
{
	// �ڽĵ��� ���� �� �����Դϴ�.
	std::stack<Transform*> stack;
	// �ڽ� Ʈ���������� ���ÿ� ����ϴ�.
	for (auto& child : this->m_childs)
		stack.push(child);

	while (!stack.empty())
	{
		Transform* transform = stack.top();
		stack.pop();

		// �ش� Ʈ�������� ��Ȱ��ȭ ���¶�� ���� Ȱ��ȭ ���¸� �ٲ��� �ʽ��ϴ�.
		// �׸��� �� Ʈ�������� �ڽĵ��� ���ÿ� ���� �ʰ� ���� ������ ���ҷ� �Ѿ�ϴ�.
		// ������ ���� Ȱ��ȭ ���°� ��Ȱ��ȭ �����̹Ƿ� �� Ʈ�������� �ڽĵ� ���� ���� ��Ȱ��ȭ ���´� ��Ȱ��ȭ ������ ���Դϴ�.
		if (transform->GetEnable() == false) continue;

		bool prev = transform->m_parentEnable;

		// �ش� Ʈ�������� Ȱ��ȭ �����̰� �θ� ���� Ȱ��ȭ ���¶�� ���� Ȱ��ȭ ���� ��Ȱ��ȭ ���·� �ٲ�ϴ�.
		transform->m_parentEnable = false;

		if (prev != transform->m_parentEnable)
		{
			if (m_parentEnable) transform->GetGameObject()->OnEnabledParent();
			else transform->GetGameObject()->OnDisabledParent();
		}

		// �ڽ� Ʈ���������� ���ÿ� ����ϴ�.
		for (auto& child : transform->m_childs)
			stack.push(child);
	}
}

void Transform::OnEnable()
{
	// �ڽ� Ʈ���������� ���� Ȱ��ȭ ���¸� Ȱ��ȭ�մϴ�.
	// 
	// Ʈ�������� ���� Ȱ��ȭ ���¸� �����մϴ�.
	UpdateParentEnableOfThis();

	// Ʈ�������� �����Ǿ� �ִ� ���ӿ�����Ʈ�� Ȱ��ȭ ���·� �����մϴ�.
	GAMEOBJECT->SetEnable(true);

	ActiveParentEnableOfChilds();
}

void Transform::OnDisable()
{
	// �ڽ� Ʈ���������� ���� Ȱ��ȭ ���¸� ��Ȱ��ȭ�մϴ�.

	// Ʈ�������� ���� Ȱ��ȭ ���¸� �����մϴ�.
	UpdateParentEnableOfThis();

	// Ʈ�������� �����Ǿ� �ִ� ���ӿ�����Ʈ�� ��Ȱ��ȭ ���·� �����մϴ�.
	GAMEOBJECT->SetEnable(false);

	DeactiveParentEnableOfChilds();
}

void Transform::OnDestroy()
{
	// Ʈ�������� �ı��Ǵ� ��쿡 ȣ��˴ϴ�.

	// Ʈ������ ������Ʈ�� ������ ���
	// �����Ǿ� �ִ� ���ӿ�����Ʈ ���� �����մϴ�.
	GAMEOBJECT->Destroy();

	// ��� �ڽ� Ʈ�������� �����մϴ�.
	while (!m_childs.empty())
	{
		auto child = m_childs.back();
		child->Destroy();
	}

	// �θ� �ִ� ���
	// �θ��� �ڽĿ��� Ż���մϴ�.
	if (m_parent)
		m_parent->DetachChild(this);

	// ��� �ڽ� Ʈ�������� Ż���ŵ�ϴ�.
	DetachAllChilds();
}
