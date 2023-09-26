#pragma once

class Object abstract
{
	friend class GameObject;

	PROTECTED Object();
	PROTECTED virtual ~Object();

	PROTECTED virtual void OnEnable() {};
	PROTECTED virtual void OnDisable() {};
	PROTECTED virtual void OnDestroy() {};

	PUBLIC void Destroy();
	PUBLIC bool IsShouldDestroy() const;
	PUBLIC void SetEnable(bool enable);
	PUBLIC bool GetEnable() const;

	PRIVATE bool m_shouldDestroy;
	PRIVATE bool m_enable;

	// ������Ʈ�� Destory() �Լ��� ȣ��Ǹ� �� �ѹ� ȣ��Ǵ� �ݹ��Դϴ�.
	PUBLIC Delegate<void, Object*> OnDestoryCallback;

	// ������Ʈ�� Ȱ��ȭ�� ����Ǿ��� �� ȣ��Ǵ� �ݹ��Դϴ�.
	// ���� ���·� ����Ǿ��� ���� ȣ����� �ʽ��ϴ�.
	PUBLIC Delegate<void, bool, Object*> OnEnableChangeCallback;
};