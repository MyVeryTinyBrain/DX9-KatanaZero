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

	// �� �̺�Ʈ �Լ������� ȣ�� �����Դϴ�.
	// ���� �������� ���� ȣ��Ǹ� Ŭ ���� �ʰ� ȣ��˴ϴ�.
	PROTECTED int m_beforeFixedStepExecutionOrder;
	PROTECTED int m_fixedUpdateExecutionOrder;
	PROTECTED int m_lateFixedUpdateExecutionOrder;
	PROTECTED int m_updateExecutionOrder;
	PROTECTED int m_lateUpdateExecutionOrder;
	PROTECTED int m_afterUpdateExecutionOrder;
	PROTECTED int m_beforeRenderExecutionOrder;
	PROTECTED int m_renderOrder;
	PROTECTED int m_afterRenderOrder;

	// �ش� �÷��װ� ���� �ִ� ��쿡��
	// �ش� �÷��׿� ��ġ�ϴ� �Լ��� ȣ����� �ʽ��ϴ�.
	// �⺻������ �����ڿ��� ��� Ȱ��ȭ �˴ϴ�.
	// ������ AddComponent() �Լ����� �����ǹǷ� DontUse##FucntionName() �Լ��� ȣ��Ǿ� ��Ȱ��ȭ �˴ϴ�.
	// COMPONENT_OVERRIDE() ��ũ�θ� ���� �������̵� �� ��쿡�� DontUse##FucntionName() �Լ��� ������ ����˴ϴ�.
	// ���� �������̵� �� ��쿡�� �÷��׸� ��Ȱ��ȭ ���� �ʰ� �˴ϴ�.

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

	// ������Ʈ�� ������ �� �����Ǵ� �������Դϴ�.
	// ������Ʈ�� ������ ���ӿ�����Ʈ�� Ʈ�������� ������ �����ϰ� ���ݴϴ�.

	PRIVATE GameObject* m_gameObject;
	PRIVATE Transform* m_transform;
};

#include "Component.hpp"
