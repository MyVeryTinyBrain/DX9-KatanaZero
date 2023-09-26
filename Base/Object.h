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

	// 오브젝트의 Destory() 함수가 호출되면 단 한번 호출되는 콜백입니다.
	PUBLIC Delegate<void, Object*> OnDestoryCallback;

	// 오브젝트의 활성화가 변경되었을 때 호출되는 콜백입니다.
	// 같은 상태로 변경되었을 때는 호출되지 않습니다.
	PUBLIC Delegate<void, bool, Object*> OnEnableChangeCallback;
};