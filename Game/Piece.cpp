#include "stdafx.h"
#include "Piece.h"
#include "GameLayer.h"
#include "GameRenderOrder.h"
#include "GameTag.h"
#include "DebugDraw.h"

void Piece::Awake()
{
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_PIECE);

	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Dynamic);
	m_body->SetLayerIndex(LAYERINDEX_GROUND_OBJECT);
	m_body->SetInterpolation(false);
	m_body->SetGravityScale(0.4f);
	m_body->SetDensity(10);

	auto colObj = GameObject::CreateChild(GetGameObject());
	m_collider = colObj->AddComponent<BoxCollider>();
	m_collider->SetWidth(2);
	m_collider->SetHeight(2);
	m_collider->SetRestitution(0.2f);
	m_collider->SetFriction(0.75f);
	m_collider->DrawDebug = DRAW_OBJECT;
}

void Piece::Update()
{
	if (m_elapsed > 4.0f)
	{
		if (m_body && m_body->GetVelocity().magnitude() < 5.0f)
		{
			m_body->Destroy();
			m_collider->Destroy();
			m_body = nullptr;
			m_collider = nullptr;
			return;
		}
	}

	m_elapsed += Time::GetDeltaTime();
}

void Piece::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

void Piece::SetSprite(const wstring& key)
{
	m_sprite = new Sprite(key);
	m_renderer->SetSprite(m_sprite);

	if (m_sprite->GetTexture())
	{
		m_collider->SetWidth(m_sprite->GetSize().x);
		m_collider->SetHeight(m_sprite->GetSize().y);
	}
}

void Piece::SetVelocity(const Vec2& velocity)
{
	m_body->SetVelocity(velocity);
	float rad = Vec2::Angle180(velocity) * DEG2RAD;
	float av = rad * float(rand() % 10 - 5);
	m_body->SetAngularVelocity(av);
}

void Piece::SetPower(const Vec2& direction, float power, float angleRange)
{
	float angle = fmodf(float(rand()), angleRange);
	Vec2 dir = Quat::AxisAngle(Vec3(0,0,1), angle * DEG2RAD) * direction;
	Vec2 vel = dir * power;
	float av = angle * DEG2RAD;
	m_body->SetVelocity(vel);
	m_body->SetAngularVelocity(av);
}

void Piece::SetTorque(float torque)
{
	m_body->SetAngularVelocity(torque);
}

Body* Piece::GetBody() const
{
	return m_body;
}

void Piece::SetColliderSize(const Vec2& size)
{
	m_collider->SetWidth(size.x);
	m_collider->SetHeight(size.y);
}
