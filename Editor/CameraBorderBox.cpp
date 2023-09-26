#include "editor_stdafx.h"
#include "CameraBorderBox.h"
#include "EditorManager.h"
#include "Background.h"

void CameraBorderBox::Awake()
{
	TransformableRect::Awake();

	m_useCenter = false;
	m_removable = false;
	m_lineColor = Color(1, 0, 0, 1);
}

void CameraBorderBox::Update()
{
	m_useLeft = m_useRight = m_useTop = m_useBottom = m_show;

	TransformableRect::Update();
}

void CameraBorderBox::Render()
{
	TransformableRect::Render();
}

Json::Value CameraBorderBox::ToJson() const
{
	Json::Value value;

	/*float*/	value["position_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["position_y"] = GetTransform()->GetPosition().y;
	/*int*/		value["width"] = GetWidth();
	/*int*/		value["height"] = GetHeight();

	return value;
}

string CameraBorderBox::JsonType() const
{
	return "camera_border";
}

void CameraBorderBox::Reset()
{
	SetSize(10000, 10000);
	GetTransform()->SetPosition(Vec2::zero());
	m_show = false;
}

void CameraBorderBox::FitToBackground()
{
	auto front_bg = EditorManager::GetInstance()->GetFrontBackground();
	auto middle_bg = EditorManager::GetInstance()->GetMiddleBackground();
	auto back_bg = EditorManager::GetInstance()->GetBackBackground();
	Vec2 front_size = front_bg->GetScaledSize();
	Vec2 middle_size = middle_bg->GetScaledSize();
	Vec2 back_size = back_bg->GetScaledSize();

	Background* max_bg = front_bg;
	float max_size = front_size.magnitude();
	if (middle_size.magnitude() > max_size)
	{
		max_bg = middle_bg;
		max_size = middle_size.magnitude();
	}
	if (back_size.magnitude() > max_size)
	{
		max_bg = back_bg;
		max_size = back_size.magnitude();
	}

	SetSize(int(max_bg->GetScaledSize().x), int(max_bg->GetScaledSize().y));
}
