#include "base_stdafx.h"
#include "Object.h"

Object::Object()
{
	// �ʱ⿡ Ȱ��ȭ ���´� ���� �ֽ��ϴ�.
	// ������Ʈ ���� �� SetEnable() �Լ��� ���� Ȱ��ȭ �����ְ� �˴ϴ�.
	// �̿� ���� OnEnable() �Լ��� ȣ��˴ϴ�.
	m_enable = false;
	m_shouldDestroy = false;
}

Object::~Object()
{
}

void Object::Destroy()
{
	// �̹� �ı��� ������ ����̸� �Լ��� �����մϴ�.
	if (m_shouldDestroy) return;

	// �ı��� �����̶�� �������ݴϴ�.
	m_shouldDestroy = true;

	// Ȱ��ȭ ���¸� ���ϴ�.
	SetEnable(false);

	// �̺�Ʈ �ݹ��� ȣ���մϴ�.
	OnDestoryCallback(this);

	// �̺�Ʈ �Լ��� ȣ���մϴ�.
	OnDestroy();
}

bool Object::IsShouldDestroy() const
{
	return m_shouldDestroy;
}

void Object::SetEnable(bool enable)
{
	// ���� ���·� �ٲٷ��� ��� �Լ��� �����մϴ�.
	if (m_enable == enable) return;

	m_enable = enable;

	// �ٲ���� ���¿� ���� �̺�Ʈ �Լ��� ȣ���մϴ�.
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
