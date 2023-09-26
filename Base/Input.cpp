#include "base_stdafx.h"
#include "Input.h"

void Input::SetKeyDown(unsigned char key)
{
	if (m_keyState[key])
		return;
	m_hasDown = true;
	m_downState[key] = true;
	m_keyState[key] = true;
}

void Input::SetKeyUp(unsigned char key)
{
	if (!m_keyState[key])
		return;
	m_hasUp = true;
	m_upState[key] = true;
	m_keyState[key] = false;
}

void Input::AddWheelDelta(short wParamHIWORD)
{
	if (wParamHIWORD > 0) ++m_wheelDelta;
	else --m_wheelDelta;
}

void Input::SetUsed()
{
	if (m_hasDown)
		ZeroMemory(m_downState, sizeof(m_downState));
	if (m_hasUp)
		ZeroMemory(m_upState, sizeof(m_upState));
	m_hasDown = false;
	m_hasUp = false;
	m_wheelDelta = 0;
}

Input Input::g_instance;

Input* Input::GetInstance()
{
	return &g_instance;
}

const bool& Input::GetKey(const Key& key)
{
	return g_instance.m_keyState[(unsigned char)key];
}

const bool& Input::GetKeyDown(const Key& key)
{
	return g_instance.m_downState[(unsigned char)key];
}

const bool& Input::GetKeyUp(const Key& key)
{
	return g_instance.m_upState[(unsigned char)key];
}

const float& Input::GetMouseWheelDelta()
{
	return g_instance.m_wheelDelta;
}
