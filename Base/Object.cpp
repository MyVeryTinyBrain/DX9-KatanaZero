#include "base_stdafx.h"
#include "Object.h"

Object::Object()
{
	// 초기에 활성화 상태는 꺼져 있습니다.
	// 오브젝트 생성 후 SetEnable() 함수를 통해 활성화 시켜주게 됩니다.
	// 이에 따라서 OnEnable() 함수도 호출됩니다.
	m_enable = false;
	m_shouldDestroy = false;
}

Object::~Object()
{
}

void Object::Destroy()
{
	// 이미 파괴될 예정인 경우이면 함수를 종료합니다.
	if (m_shouldDestroy) return;

	// 파괴될 예정이라고 설정해줍니다.
	m_shouldDestroy = true;

	// 활성화 상태를 끕니다.
	SetEnable(false);

	// 이벤트 콜백을 호출합니다.
	OnDestoryCallback(this);

	// 이벤트 함수를 호출합니다.
	OnDestroy();
}

bool Object::IsShouldDestroy() const
{
	return m_shouldDestroy;
}

void Object::SetEnable(bool enable)
{
	// 같은 상태로 바꾸려는 경우 함수를 종료합니다.
	if (m_enable == enable) return;

	m_enable = enable;

	// 바뀌려는 상태에 따라 이벤트 함수를 호출합니다.
	if (enable)
	{
		OnEnableChangeCallback(true, this);
		OnEnable();
	}
	else
	{
		OnEnableChangeCallback(false, this);
		OnDisable();
	}
}

bool Object::GetEnable() const
{
	return m_enable;
}
