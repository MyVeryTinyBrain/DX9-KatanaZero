#include "editor_stdafx.h"
#include "TriggerBox.h"

std::vector<TriggerBox*> TriggerBox::triggers;

void TriggerBox::Awake()
{
    TransformableRect::Awake();

	m_lineColor = Color(0, 0, 1, 1);

	triggers.push_back(this);
}

void TriggerBox::Render()
{
    TransformableRect::Render();

	auto text = GraphicDevice::GetTextDevice();
	WriteTextParameters param;
	param.position = GetTransform()->GetPosition();
	param.hAlignment = TextHAlignment::Center;
	text->WriteText(m_name, &param);
}

void TriggerBox::OnDestroy()
{
	TransformableRect::OnDestroy();

	auto it = std::find(triggers.begin(), triggers.end(), this);
	if (it == triggers.end()) return;
	triggers.erase(it);
}

Json::Value TriggerBox::ToJson() const
{
	USES_CONVERSION;
	string s_name(W2A(m_name.c_str()));

	Json::Value value;

	/*string*/	value["name"] = s_name;
	/*float*/	value["position_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["position_y"] = GetTransform()->GetPosition().y;
	/*int*/		value["width"] = GetWidth();
	/*int*/		value["height"] = GetHeight();

	return value;
}

string TriggerBox::JsonType() const
{
	return "trigger";
}
