#include "stdafx.h"
#include "EndArrow.h"
#include "Enemy.h"
#include "Trigger.h"

void EndArrow::Awake()
{
	DownArrow::Awake();

	m_renderer->GetTransform()->SetAngle(90);
}

void EndArrow::Start()
{
	DownArrow::Start();

	m_renderer->SetEnable(false);

	Trigger* endTrigger = Trigger::FindWithName(L"End");
	m_hasEnd = false;
	if (endTrigger)
	{
		m_hasEnd = true;
		m_endPos = endTrigger->GetTransform()->GetPosition();
	}
}

void EndArrow::Update()
{
	if (!m_hasEnd) return;
	if (Enemy::g_enemies.size() > 0) return;

	auto fsign = [](float f)
	{
		return f < 0 ? -1 : +1.0f;
	};

	m_renderer->SetEnable(true);

	auto cam = CCamera2D::GetInstance();
	const Vec2& camPos = cam->GetPosition();
	Vec2 rel = m_endPos - camPos;
	Vec2 absRel = Vec2(fabsf(rel.x), fabsf(rel.y));

	float width = cam->GetWidth();
	float height = cam->GetHeight();
	float hw = width * 0.5f;
	float hh = height * 0.5f;

	float yPos = m_endPos.y;
	if (absRel.y > (hh - 10.0f))
	{
		rel.y = (hh - 10.0f) * fsign(rel.y);
	}

	float xPos = m_endPos.x;
	if (absRel.x > (hw - 12.0f))
	{
		rel.x = (hw - 12.0f) * fsign(rel.x);
	}

	GetTransform()->SetPosition(camPos + rel);

	float localY = 20 + sinf(10 * m_elapsed) * 10;
	m_renderer->GetTransform()->SetUp(Vec2(-fsign(rel.x), 0));
	m_renderer->GetTransform()->SetLocalPosition(m_renderer->GetTransform()->GetUp() * localY);

	m_elapsed += Time::GetDeltaTime();
}
