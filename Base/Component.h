#pragma once

#include "Object.h"

class GameObject;
class Transform;

class Component abstract : public Object
{
	friend class GameObject;
	friend class Job;

	PROTECTED Component();
	PROTECTED virtual ~Component();

	PROTECTED virtual void Awake() {};
	PROTECTED virtual void Start() {};
	PROTECTED virtual void BeforeFixedStep() {};
	PROTECTED virtual void FixedUpdate() {};
	PROTECTED virtual void LateFixedUpdate() {};
	PROTECTED virtual void Update() {};
	PROTECTED virtual void LateUpdate() {};
	PROTECTED virtual void AfterUpdate() {};
	PROTECTED virtual void BeforeRender() {};
	PROTECTED virtual void Render() {};
	PROTECTED virtual void AfterRender() {};
	PROTECTED virtual void OnEnabledGameObject() {};
	PROTECTED virtual void OnDisabledGameObject() {};

	PROTECTED virtual void DontUseStart() { m_callStart = false; }
	PROTECTED virtual void DontUseBeforeFixedStep() { m_callBeforeFixedStep = false; }
	PROTECTED virtual void DontUseFixedUpdate() { m_callFixedUpdate = false; }
	PROTECTED virtual void DontUseLateFixedUpdate() { m_callLateFixedUpdate = false; }
	PROTECTED virtual void DontUseUpdate() { m_callUpdate = false; }
	PROTECTED virtual void DontUseLateUpdate() { m_callLateUpdate = false; }
	PROTECTED virtual void DontUseAfterUpdate() { m_callAfterUpdate = false; }
	PROTECTED virtual void DontUseBeforeRender() { m_callBeforeRender = false; }
	PROTECTED virtual void DontUseRender() { m_callRender = false; }
	PROTECTED virtual void DontUseAfterRender() { m_callAfterRender = false; }

	PUBLIC template <class ComponentType> ComponentType* AddComponent();
	PUBLIC template <class ComponentType> bool RemoveComponent();
	PUBLIC bool RemoveComponent(Component* component);
	PUBLIC template <class ComponentType> ComponentType* GetComponent();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInChild();
	PUBLIC template <class ComponentType> ComponentType* GetComponentInParent();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponents();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInChild();
	PUBLIC template <class ComponentType> std::vector<ComponentType*> GetComponentsInParent();

	PUBLIC GameObject* GetGameObject() const;
	PUBLIC Transform* GetTransform() const;

	PUBLIC bool GetActive() const;
	PUBLIC int GetBeforeFixedStepExecutionOrder() const;
	PUBLIC int GetFixedExecutionOrder() const;
	PUBLIC int GetLateFixedExecutionOrder() const;
	PUBLIC int GetUpdateExecutionOrder() const;
	PUBLIC int GetLateUpdateExecutionOrder() const;
	PUBLIC int GetAfterUpdateExecutionOrder() const;
	PUBLIC int GetBeforeRenderExecutionOrder() const;
	PUBLIC int GetRenderOrder() const;

	// 각 이벤트 함수에서의 호출 순서입니다.
	// 값이 작을수록 먼저 호출되며 클 수록 늦게 호출됩니다.
	PROTECTED int m_beforeFixedStepExecutionOrder;
	PROTECTED int m_fixedUpdateExecutionOrder;
	PROTECTED int m_lateFixedUpdateExecutionOrder;
	PROTECTED int m_updateExecutionOrder;
	PROTECTED int m_lateUpdateExecutionOrder;
	PROTECTED int m_afterUpdateExecutionOrder;
	PROTECTED int m_beforeRenderExecutionOrder;
	PROTECTED int m_renderOrder;
	PROTECTED int m_afterRenderOrder;

	// 해당 플래그가 꺼져 있는 경우에는
	// 해당 플래그와 일치하는 함수가 호출되지 않습니다.
	// 기본적으로 생성자에서 모두 활성화 됩니다.
	// 하지만 AddComponent() 함수에서 생성되므로 DontUse##FucntionName() 함수가 호출되어 비활성화 됩니다.
	// COMPONENT_OVERRIDE() 매크로를 통해 오버라이딩 한 경우에는 DontUse##FucntionName() 함수의 내용이 변경됩니다.
	// 따라서 오버라이딩 한 경우에는 플래그를 비활성화 하지 않게 됩니다.

	PRIVATE bool m_callStart;
	PRIVATE bool m_callBeforeFixedStep;
	PRIVATE bool m_callFixedUpdate;
	PRIVATE bool m_callLateFixedUpdate;
	PRIVATE bool m_callUpdate;
	PRIVATE bool m_callLateUpdate;
	PRIVATE bool m_callAfterUpdate;
	PRIVATE bool m_callBeforeRender;
	PRIVATE bool m_callRender;
	PRIVATE bool m_callAfterRender;

	// 컴포넌트가 생성될 때 설정되는 변수들입니다.
	// 컴포넌트가 부착된 게임오브젝트와 트랜스폼에 접근이 가능하게 해줍니다.

	PRIVATE GameObject* m_gameObject;
	PRIVATE Transform* m_transform;
};

#include "Component.hpp"
