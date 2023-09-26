#include "editor_stdafx.h"
#include "TransformableRect.h"

#define RADIUS 5

TransformableRect* TransformableRect::g_select = nullptr;

void TransformableRect::Awake()
{
	float h = 0.5f;

	auto left = new GameObject(GetTransform());
	auto top = new GameObject(GetTransform());
	auto right = new GameObject(GetTransform());
	auto bottom = new GameObject(GetTransform());

	m_left = left->GetTransform();
	m_top = top->GetTransform();
	m_right = right->GetTransform();
	m_bottom = bottom->GetTransform();

	m_selection = SEL_NONE;

	SetSize(32, 32);

	m_useLeft = true;
	m_useTop = true;
	m_useRight = true;
	m_useBottom = true;
	m_useCenter = true;
	m_removable = true;

	m_lineColor = Color(1, 1, 1, 0.75f);
}

void TransformableRect::Update()
{
	if (Input::GetKey(Key::Space))
	{
		PutHandle();
		return;
	}

	if (Input::GetKeyDown(Key::RightMouse)) PickDelete();
	if (Input::GetKeyDown(Key::LeftMouse)) PickHandle();
	if (!Input::GetKey(Key::LeftMouse)) PutHandle();

	Handling();
}

void TransformableRect::LateUpdate()
{
	float world_left = m_left->GetPosition().x;
	float world_top = m_top->GetPosition().y;
	float world_right = m_right->GetPosition().x;
	float world_bottom = m_bottom->GetPosition().y;

	float left = m_left->GetLocalPosition().x;
	float top = m_top->GetLocalPosition().y;
	float right = m_right->GetLocalPosition().x;
	float bottom = m_bottom->GetLocalPosition().y;

	float absLeft = fabsf(left);
	float absTop = fabsf(top);
	float absRight = fabsf(right);
	float absBottom = fabsf(bottom);

	float width = absLeft + absRight;
	float height = absTop + absBottom;

	bool changed = false;
	if (width != m_width)
	{
		changed = true;
	}
	if (height != m_height)
	{
		changed = true;
	}

	if (changed)
	{
		Vec2 center = Vec2(
			m_left->GetTransform()->GetPosition().x + m_right->GetTransform()->GetPosition().x,
			m_bottom->GetTransform()->GetPosition().y + m_top->GetTransform()->GetPosition().y) * 0.5f;
		GetTransform()->SetPosition(center);
		SetSize(int(width), int(height));
	}
}

void TransformableRect::Render()
{
	LineDevice* line = GraphicDevice::GetLineDevice();
	const Vec2& p = GetTransform()->GetPosition();
	const float& x = p.x;
	const float& y = p.y;
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;

	Draw2DLineParameters param;
	param.color = m_lineColor;

	if (m_useLeft || m_useTop || m_useRight || m_useBottom) line->Draw2DRect(x - hw, y + hh, x + hw, y - hh, &param);
	if (m_useLeft) line->Draw2DCircle(left(), RADIUS, &param);
	if (m_useTop)line->Draw2DCircle(top(), RADIUS, &param);
	if (m_useRight)line->Draw2DCircle(right(), RADIUS, &param);
	if (m_useBottom)line->Draw2DCircle(bottom(), RADIUS, &param);
	if (m_useCenter) line->Draw2DCircle(center(), RADIUS, &param);
}

void TransformableRect::OnDestroy()
{
	if (g_select == this)
	{
		g_select = nullptr;
	}
}

void TransformableRect::SetSize(int width, int height)
{
	float hw = width * 0.5f;
	float hh = height * 0.5f;

	m_left->SetLocalPosition(Vec2::left() * hw);
	m_right->SetLocalPosition(Vec2::right() * hw);
	m_top->SetLocalPosition(Vec2::up() * hh);
	m_bottom->SetLocalPosition(Vec2::down() * hh);

	m_width = width;
	m_height = height;
}

int TransformableRect::GetWidth() const
{
	return m_width;
}

int TransformableRect::GetHeight() const
{
	return m_height;
}

void TransformableRect::PickDelete()
{
	if (!m_removable) return;
	if (!m_useCenter) return;

	Vec2 mp = CCamera2D::GetInstance()->GetMousePositionInWorld();
	if (Vec2::Distance(mp, center()) < RADIUS)
	{
		if (g_select == this)
		{
			g_select = nullptr;
		}
		GetGameObject()->Destroy();
	}
}

void TransformableRect::PickHandle()
{
	if (g_select) return;

	Vec2 mp = CCamera2D::GetInstance()->GetMousePositionInWorld();
	if (m_useLeft && Vec2::Distance(mp, left()) < RADIUS)
	{
		m_selection = SEL_LEFT;
	}
	else if (m_useTop && Vec2::Distance(mp, top()) < RADIUS)
	{
		m_selection = SEL_TOP;
	}
	else if (m_useRight && Vec2::Distance(mp, right()) < RADIUS)
	{
		m_selection = SEL_RIGHT;
	}
	else if (m_useBottom && Vec2::Distance(mp, bottom()) < RADIUS)
	{
		m_selection = SEL_BOTTOM;
	}
	else if (m_useCenter && Vec2::Distance(mp, center()) < RADIUS)
	{
		m_selection = SEL_CENTER;
	}

	if (m_selection != SEL_NONE)
	{
		g_select = this;
	}
}

void TransformableRect::PutHandle()
{
	if (m_selection == SEL_NONE) return;
	if (g_select == this)
	{
		g_select = nullptr;
	}
	m_selection = SEL_NONE;
}

void TransformableRect::Handling()
{
	if (m_selection == SEL_NONE) return;
	Vec2 mp = CCamera2D::GetInstance()->GetMousePositionInWorld();
	Vec2 cp = GetTransform()->GetPosition();
	mp.x = float(int(mp.x));
	mp.y = float(int(mp.y));
	cp.x = float(int(cp.x));
	cp.y = float(int(cp.y));
	switch (m_selection)
	{
		case SEL_LEFT:
			m_left->SetPosition(Vec2(mp.x, cp.y));
			break;
		case SEL_TOP:
			m_top->SetPosition(Vec2(cp.x, mp.y));
			break;
		case SEL_RIGHT:
			m_right->SetPosition(Vec2(mp.x, cp.y));
			break;
		case SEL_BOTTOM:
			m_bottom->SetPosition(Vec2(cp.x, mp.y));
			break;
		case SEL_CENTER:
			GetTransform()->SetPosition(mp);
			break;
	}
}

Vec2 TransformableRect::center() const
{
	return GetTransform()->GetPosition();
}

Vec2 TransformableRect::left() const
{
	const Vec2& p = GetTransform()->GetPosition();
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return p + Vec2::left() * hw;
}

Vec2 TransformableRect::top() const
{
	const Vec2& p = GetTransform()->GetPosition();
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return p + Vec2::up() * hh;
}

Vec2 TransformableRect::right() const
{
	const Vec2& p = GetTransform()->GetPosition();
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return p + Vec2::right() * hw;
}

Vec2 TransformableRect::bottom() const
{
	const Vec2& p = GetTransform()->GetPosition();
	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;
	return p + Vec2::down() * hh;
}
