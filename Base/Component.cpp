#include "base_stdafx.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

Component::Component() : Object()
{
	// �����ڿ����� ��� �Լ��� ȣ���Ҽ� �ְ� �÷��װ� �����˴ϴ�.
	// ������ ���ӿ�����Ʈ���� AddComponent() �Լ��� ���� �����ǹǷ�
	// DontUse##FunctionName() �Լ��� ȣ��Ǿ� �� �÷��װ� �����ϴ�.
	// �Լ��� COMPONENT_OVERRIDE() ��ũ�θ� ���� �������̵��� ��� 
	// DontUse##FunctionName() �Լ��� ������ ������ �� �÷��װ� ������ �ʽ��ϴ�.
	m_callStart = true;
	m_callBeforeFixedStep = true;
	m_callFixedUpdate = true;
	m_callLateFixedUpdate = true;
	m_callUpdate = true;
	m_callLateUpdate = true;
	m_callAfterUpdate = true;
	m_callBeforeRender = true;
	m_callRender = true;
	m_callAfterRender = true;

	m_beforeFixedStepExecutionOrder = 0;
	m_fixedUpdateExecutionOrder = 0;
	m_lateFixedUpdateExecutionOrder = 0;
	m_updateExecutionOrder = 0;
	m_lateUpdateExecutionOrder = 0;
	m_afterUpdateExecutionOrder = 0;
	m_beforeRenderExecutionOrder = 0;
	m_renderOrder;

	m_gameObject = nullptr;
	m_transform = nullptr;
}

Component::~Component()
{

}

GameObject* Component::GetGameObject() const
{
	return m_gameObject;
}

Transform* Component::GetTransform() const
{
	return m_transform;
}

bool Component::GetActive() const
{
	return GetGameObject()->GetActive() && GetEnable();
}

int Component::GetBeforeFixedStepExecutionOrder() const
{
	return m_beforeFixedStepExecutionOrder;
}

int Component::GetFixedExecutionOrder() const
{
	return m_fixedUpdateExecutionOrder;
}

int Component::GetLateFixedExecutionOrder() const
{
	return m_lateFixedUpdateExecutionOrder;
}

int Component::GetUpdateExecutionOrder() const
{
	return m_updateExecutionOrder;
}

int Component::GetLateUpdateExecutionOrder() const
{
	return m_lateUpdateExecutionOrder;
}

int Component::GetAfterUpdateExecutionOrder() const
{
	return 0;
}

int Component::GetBeforeRenderExecutionOrder() const
{
	return 0;
}

int Component::GetRenderOrder() const
{
	return m_renderOrder;
}
