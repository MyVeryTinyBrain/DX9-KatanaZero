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
	// y축을 나타내는 변환된 표준 기저를 반환합니다.
	return LocalToWorldMatrix().MultiplyVector(Vec2::up());
}

Vec2 Transform::GetRight() const
{
	// x축을 나타내는 변환된 표준 기저를 반환합니다.
	return LocalToWorldMatrix().MultiplyVector(Vec2::right());
}

void Transform::SetUp(const Vec2& up)
{
	// y축을 나타내는 표준 기저가 매개변수 up 방향을 바라보도록 각도를 변경합니다.
	Vec2 yBasis = up.normalized();
	float angle = Quat::FromToRotation(Vec2::up(), yBasis).GetEulerAngle().z;
	SetAngle(angle);
}

void Transform::SetRight(const Vec2& right)
{
	// x축을 나타내는 표준 기저가 매개변수 right 방향을 바라보도록 각도를 변경합니다.
	Vec2 xBasis = right.normalized();
	float angle = Quat::FromToRotation(Vec2::right(), xBasis).GetEulerAngle().z;
	SetAngle(angle);
}

void Transform::ForceUpdateChilds(Transform* root)
{
	// 자식 객체들의 월드 트랜스폼을 갱신합니다.

	// root가 nullptr인 경우
	// 이 트랜스폼의 최상단 부모를 root로 설정합니다.
	if (root == nullptr) root = GetRoot();

	// 자식들이 담기게 될 스택입니다.
	std::stack<Transform*> stack;

	stack.push(root);

	// root의 모든 자식들의 월드 트랜스폼을 갱신합니다.
	// root의 월드/로컬 트랜스폼이 변경된 경우에
	// 바뀌어야 할 것은 월드 트랜스폼 뿐입니다.
	// 따라서 월드 트랜스폼만을 갱신합니다.
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
	// 같은 부모로 설정하려는 경우 연산을 줄이기 위해 함수를 종료합니다.
	if (transform == m_parent) return;

	// 이미 부모가 있는 경우에
	// 그 부모에게서 탈퇴합니다.
	if (m_parent)
		m_parent->DetachChild(this);

	// 설정하려는 부모가 nullptr이 아닌 경우
	// 이 트랜스폼을 자식으로 추가합니다.
	// AddChild 함수에서 이 트랜스폼의 m_parent를 설정합니다.
	if (transform)
		transform->AddChild(this);
	// 설정하려는 부모가 nullptr인 경우
	// m_parent를 nullptr로 설정합니다.
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
	// 이 트랜스폼을 포함하고
	// 또한 모든 자식들을 모두 포함하는 벡터입니다.
	// 단순히 이 트랜스폼의 자식들이 아닌 자식들의 자식들까지 모두 담습니다.
	std::vector<Transform*> v;

	std::stack<const Transform*> stack;
	stack.push(this);

	while (!stack.empty())
	{
		const Transform* transform = stack.top();
		stack.pop();

		// 자식 트랜스폼들을 스택에 담습니다.
		for (auto& child : transform->m_childs)
			stack.push(child);

		// 트랜스폼을 벡터에 담습니다.
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
	// 해제하려는 트랜스폼이 nullptr인 경우 함수를 종료합니다.
	if (transform == nullptr) return;

	// 매개변수의 트랜스폼이 자식이 아니라면 함수를 종료합니다.
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

	// 보조 활성화 상태를 설정합니다.
	// 보조 활성화 상태를 참조해 부모가 비활성화 상태인 경우에 자식도 비활성화 상태로 인식할 수 있습니다.
	// 메인 활성화 상태와 보조 활성화 상태가 모두 활성화 되어있을때 활성화 상태로 인식하도록 합니다.

	if (m_parent)
	{
		// 부모가 있는 경우
		// 부모가 비활성화 상태이거나, 부모의 부모가 비활성화 상태인경우 비활성화로 설정합니다.
		// 부모가 활성화 상태이고, 부모의 부모 역시 활성화 상태이면 활성화로 설정합니다.
		m_parentEnable = m_parent->GetEnable() && m_parent->GetParentEnable();
	}
	else
	{
		// 부모가 없는 경우
		// 활성화로 설정합니다.
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
	// 자식들이 담기게 될 스택입니다.
	std::stack<Transform*> stack;
	stack.push(this);

	while (!stack.empty())
	{
		Transform* transform = stack.top();
		stack.pop();

		// 해당 트랜스폼이 비활성화 상태라면 보조 활성화 상태를 바꾸지 않습니다.
		// 그리고 이 트랜스폼의 자식들은 스택에 담지 않고 다음 스택의 원소로 넘어갑니다.
		// 어차피 메인 활성화 상태가 비활성화 상태이므로 이 트랜스폼과 자식들 또한 보조 비활성화 상태는 비활성화 상태일 것입니다.
		if (transform->GetEnable() == false) continue;

		bool prev = transform->m_parentEnable;

		// 해당 트랜스폼이 활성화 상태이고 부모 역시 활성화 상태라면 보조 활성화 역시 활성화 상태로 바뀝니다.
		transform->m_parentEnable = true;

		if (prev != transform->m_parentEnable)
		{
			if (m_parentEnable) transform->GetGameObject()->OnEnabledParent();
			else transform->GetGameObject()->OnDisabledParent();
		}

		// 자식 트랜스폼들을 스택에 담습니다.
		for (auto& child : transform->m_childs)
			stack.push(child);
	}
}

void Transform::DeactiveParentEnableOfChilds()
{
	// 자식들이 담기게 될 스택입니다.
	std::stack<Transform*> stack;
	// 자식 트랜스폼들을 스택에 담습니다.
	for (auto& child : this->m_childs)
		stack.push(child);

	while (!stack.empty())
	{
		Transform* transform = stack.top();
		stack.pop();

		// 해당 트랜스폼이 비활성화 상태라면 보조 활성화 상태를 바꾸지 않습니다.
		// 그리고 이 트랜스폼의 자식들은 스택에 담지 않고 다음 스택의 원소로 넘어갑니다.
		// 어차피 메인 활성화 상태가 비활성화 상태이므로 이 트랜스폼과 자식들 또한 보조 비활성화 상태는 비활성화 상태일 것입니다.
		if (transform->GetEnable() == false) continue;

		bool prev = transform->m_parentEnable;

		// 해당 트랜스폼이 활성화 상태이고 부모 역시 활성화 상태라면 보조 활성화 역시 비활성화 상태로 바뀝니다.
		transform->m_parentEnable = false;

		if (prev != transform->m_parentEnable)
		{
			if (m_parentEnable) transform->GetGameObject()->OnEnabledParent();
			else transform->GetGameObject()->OnDisabledParent();
		}

		// 자식 트랜스폼들을 스택에 담습니다.
		for (auto& child : transform->m_childs)
			stack.push(child);
	}
}

void Transform::OnEnable()
{
	// 자식 트랜스폼들의 보조 활성화 상태를 활성화합니다.
	// 
	// 트랜스폼의 보조 활성화 상태를 갱신합니다.
	UpdateParentEnableOfThis();

	// 트랜스폼이 부착되어 있는 게임오브젝트를 활성화 상태로 변경합니다.
	GAMEOBJECT->SetEnable(true);

	ActiveParentEnableOfChilds();
}

void Transform::OnDisable()
{
	// 자식 트랜스폼들의 보조 활성화 상태를 비활성화합니다.

	// 트랜스폼의 보조 활성화 상태를 갱신합니다.
	UpdateParentEnableOfThis();

	// 트랜스폼이 부착되어 있는 게임오브젝트를 비활성화 상태로 변경합니다.
	GAMEOBJECT->SetEnable(false);

	DeactiveParentEnableOfChilds();
}

void Transform::OnDestroy()
{
	// 트랜스폼이 파괴되는 경우에 호출됩니다.

	// 트랜스폼 컴포넌트를 제거한 경우
	// 부착되어 있는 게임오브젝트 역시 제거합니다.
	GAMEOBJECT->Destroy();

	// 모든 자식 트랜스폼을 제거합니다.
	while (!m_childs.empty())
	{
		auto child = m_childs.back();
		child->Destroy();
	}

	// 부모가 있는 경우
	// 부모의 자식에서 탈퇴합니다.
	if (m_parent)
		m_parent->DetachChild(this);

	// 모든 자식 트랜스폼을 탈퇴시킵니다.
	DetachAllChilds();
}
