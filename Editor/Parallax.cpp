#include "editor_stdafx.h"
#include "Parallax.h"

void Parallax::Awake()
{
	m_base = Vec2::zero();
	m_parallaxScale = Vec2(0.0f, 0.0f);
	m_scale = 1.0f;
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RD_PARALLAX_FRONT);
}

void Parallax::LateUpdate()
{
	Vec2 camPos = CCamera2D::GetInstance()->GetPosition();
	Vec2 delta = (camPos - m_base);
	delta.x *= m_parallaxScale.x;
	delta.y *= m_parallaxScale.y;

	GetTransform()->SetTransform(m_base + delta, 0.0f, Vec2::one() * m_scale);
}

void Parallax::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

Json::Value Parallax::ToJson() const
{
	USES_CONVERSION;
	string key(W2A(m_key.c_str()));
	string order;
	switch (m_renderer->GetRenderOrder())
	{
		case RD_PARALLAX_FRONT:
			order = "front";
			break;
		case RD_PARALLAX_MIDDLE:
			order = "middle";
			break;
		case RD_PARALLAX_BACK:
			order = "back";
			break;
	}

	Json::Value value;
	/*string*/	value["key"] = key;
	/*float*/	value["base_x"] = m_base.x;
	/*float*/	value["base_y"] = m_base.y;
	/*float*/	value["parallax_x"] = m_parallaxScale.x;
	/*float*/	value["parallax_y"] = m_parallaxScale.y;
	/*float*/	value["scale"] = m_scale;
	/*int*/		value["order"] = order;

	return value;
}

string Parallax::JsonType() const
{
	return "parallax";
}

void Parallax::SetBase(const Vec2& base)
{
	m_base = base;
}

void Parallax::SetParallaxScale(const Vec2& scale)
{
	m_parallaxScale = scale;
}

void Parallax::SetScale(float scale)
{
	m_scale = scale;
}

void Parallax::SetSprite(const wstring& key)
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

const Vec2& Parallax::GetBase() const
{
	return m_base;
}

const Vec2& Parallax::GetParallaxScale() const
{
	return m_parallaxScale;
}

float Parallax::GetScale() const
{
	return m_scale;
}

const wstring& Parallax::GetKey() const
{
	return m_key;
}

void Parallax::SetOrder(int order)
{
	m_renderer->SetRenderOrder(order);
}

