#include "base_stdafx.h"
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

Component::Component() : Object()
{
	// 생성자에서는 모든 함수를 호출할수 있게 플래그가 설정됩니다.
	// 하지만 게임오브젝트에서 AddComponent() 함수를 통해 생성되므로
	// DontUse##FunctionName() 함수가 호출되어 각 플래그가 꺼집니다.
	// 함수를 COMPONENT_OVERRIDE() 매크로를 통해 오버라이딩한 경우 
	// DontUse##FunctionName() 함수의 내용이 지워져 각 플래그가 꺼지지 않습니다.
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
