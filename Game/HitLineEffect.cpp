#include "stdafx.h"
#include "HitLineEffect.h"
#include "TrailRenderer.h"
#include "GameRenderOrder.h"

void HitLineEffect::Awake()
{
	//GetTransform()->SetScale(Vec2(2, 4));

	m_sprite = new Sprite(L"../Texture/effect/used/hit_line/spr_hit_line.png");

	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetSprite(m_sprite);
	m_renderer->SetRenderOrder(RENDER_ORDER_EFFECT_HIT_LINE);

	m_speed = 3000;
	m_elapsed = 0;

	m_waveform = 0.05f;
}

void HitLineEffect::Update()
{
	const Vec2& position = GetTransform()->GetPosition();
	Vec2 newPos = position + GetTransform()->GetRight() * m_speed * Time::GetUnscaledDeltaTime();
	GetTransform()->SetPosition(newPos);

	float changeTime = m_waveform;
	float colorSelect = fmod(m_elapsed, changeTime * 2.0f) / changeTime;
	int colorSelectIndex = int(colorSelect);

	Color colorA = D3DCOLOR_ARGB(255, 70, 255, 255);
	Color colorB = D3DCOLOR_ARGB(255, 200, 50, 220);

	if (colorSelect > 1.0f) colorSelect = 2.0f - colorSelect;
	Color lerp;
	D3DXColorLerp(&lerp, &colorA, &colorB, colorSelect);
	m_renderer->SetColor(lerp);

	m_elapsed += Time::GetUnscaledDeltaTime();

	if (m_elapsed > 1.0f)
	{
		GetGameObject()->Destroy();
	}
}

void HitLineEffect::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

HitLineEffect* HitLineEffect::Create(const Vec2& position, const Vec2& right)
{
	auto o = new GameObject;;
	o->GetTransform()->SetRight(right);
	auto hit = o->AddComponent<HitLineEffect>();
	float width = hit->m_sprite->GetSize().x;
	o->GetTransform()->SetPosition(position - right * width * 0.5f);
	return hit;
}
