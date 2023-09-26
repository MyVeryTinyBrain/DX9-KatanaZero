#include "editor_stdafx.h"
#include "Background.h"
#include <locale>
#include <codecvt>
#include <atlconv.h>

void Background::Awake()
{
	m_scale = 1.0f;
	m_emptyColor = Color(0, 0, 0, 1);
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RD_BACKGROUND_BACK);
}

void Background::Update()
{
	GetTransform()->SetScale(Vec2::one() * m_scale);
	GraphicDevice::SetBackgroundColor(m_emptyColor);
}

void Background::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

Json::Value Background::ToJson() const
{
	USES_CONVERSION;
	string key(W2A(m_key.c_str()));
	string order;
	switch (m_renderer->GetRenderOrder())
	{
		case RD_BACKGROUND_FRONT:
			order = "front";
			break;
		case RD_BACKGROUND_MIDDLE:
			order = "middle";
			break;
		case RD_BACKGROUND_BACK:
			order = "back";
			break;
	}

	Json::Value value;
	/*string*/	value["key"] = key;
	/*float*/	value["scale"] = m_scale;
	/*float*/	value["red"] = m_emptyColor.r;
	/*float*/	value["green"] = m_emptyColor.g;
	/*float*/	value["blue"] = m_emptyColor.b;
	/*string*/	value["order"] = order;
	/*float*/	value["adjust_x"] = GetTransform()->GetPosition().x;
	/*float*/	value["adjust_y"] = GetTransform()->GetPosition().y;

	return value;
}

string Background::JsonType() const
{
	return "background";
}

void Background::SetOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

int Background::GetOrder() const
{
	return m_renderer->GetRenderOrder();
}

void Background::SetSprite(const wstring& key)
{
	SAFE_DELETE(m_sprite);
	m_renderer->SetSprite(nullptr);

	m_key = key;
	m_sprite = new Sprite(key);

	if (!m_sprite->GetTexture())
	{
		SAFE_DELETE(m_sprite);
		return;
	}
	else
	{
		m_renderer->SetSprite(m_sprite);
	}
}

const wstring& Background::GetKey() const
{
	return m_key;
}

void Background::SetScale(float scale)
{
	m_scale = scale;
}

float Background::GetScale() const
{
	return m_scale;
}

Vec2 Background::GetScaledSize() const
{
	if (!HasSprite()) return Vec2(10, 10);
	return m_sprite->GetSize() * m_scale;
}

bool Background::HasSprite() const
{
	return m_sprite != nullptr;
}

void Background::SetEmptyColor(const Color& color)
{
	m_emptyColor = color;
}

const Color& Background::GetEmptyColor()
{
	return m_emptyColor;
}
