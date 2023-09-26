#include "stdafx.h"
#include "Boss.h"
#include "PlatformerBody.h"
#include "GameRenderOrder.h"
#include "GameTag.h"
#include "TimeController.h"
#include "TrailRenderer.h"
#include "GameLayer.h"
#include "DebugDraw.h"

void Boss::Awake()
{
	GetGameObject()->SetTag(TAG_BOSS);

	m_body = AddComponent<Body>();
	m_body->SetFixedRotationMode(true);
	m_body->SetLayerIndex(LAYERINDEX_CHARACTER);
	m_groundDamping = 10.0f;

	auto boxColObj = GameObject::CreateChild(GetGameObject());
	m_boxCollider = boxColObj->AddComponent<BoxCollider>();
	m_boxCollider->DrawDebug = DRAW_CHARACTER;

	auto cirColObj = GameObject::CreateChild(GetGameObject());
	cirColObj->GetTransform()->SetLocalPosition(Vec2::down() * m_boxCollider->GetHeight() * 0.5f);
	m_circleCollider = cirColObj->AddComponent<CircleCollider>();
	m_circleCollider->DrawDebug = DRAW_CHARACTER;

	SetWidth(12);
	SetHeight(46);

	m_boxCollider->OnCollisionEnter += Function<void, const CollisionResult&>(this, &Boss::OnCollisonEnter);
	m_boxCollider->OnCollisionExit += Function<void, Collider*>(this, &Boss::OnCollisonExit);
	m_boxCollider->OnCollisionStay += Function<void, const CollisionResult&>(this, &Boss::OnCollisonStay);
	m_boxCollider->OnTriggerEnter += Function<void, Collider*>(this, &Boss::OnTriggerEnter);
	m_boxCollider->OnTriggerExit += Function<void, Collider*>(this, &Boss::OnTriggerExit);
	m_boxCollider->OnTriggerStay += Function<void, Collider*>(this, &Boss::OnTriggerStay);

	m_circleCollider->OnCollisionEnter += Function<void, const CollisionResult&>(this, &Boss::OnCollisonEnter);
	m_circleCollider->OnCollisionExit += Function<void, Collider*>(this, &Boss::OnCollisonExit);
	m_circleCollider->OnCollisionStay += Function<void, const CollisionResult&>(this, &Boss::OnCollisonStay);
	m_circleCollider->OnTriggerEnter += Function<void, Collider*>(this, &Boss::OnTriggerEnter);
	m_circleCollider->OnTriggerExit += Function<void, Collider*>(this, &Boss::OnTriggerExit);
	m_circleCollider->OnTriggerStay += Function<void, Collider*>(this, &Boss::OnTriggerStay);

	m_rendererParentObject = GameObject::CreateChild(GetGameObject());
	auto rendererObj = GameObject::CreateChild(m_rendererParentObject);
	m_renderer = rendererObj->AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_BOSS);
#ifdef SpriteRenderer
	m_renderer->SetRecordRGBMode(false);
#endif

	m_slowModeColorChange = true;
}

void Boss::FixedUpdate()
{
	bool prevGround = m_ground;
	RaycastResult res;
	m_ground = GroundCheck(&res);
	if (m_ground && !prevGround)
	{
		OnGrounded(res.point, res.normal);
	}

	if (HasGround())
	{
		Vec2 velocity = m_body->GetVelocity();
		float m_linearDamping = m_groundDamping;
		velocity *= 1.0f / (1.0f + m_linearDamping * Time::GetFixedDeltaTime());
		m_body->SetVelocity(velocity);
	}
}

void Boss::BeforeRender()
{
	if (!m_slowModeColorChange) return;

	if (TimeController::GetInstance()->GetCurrentScale() < 0.8f)
	{
		float alpha = m_renderer->GetColor().a;
		m_renderer->SetColor(Color(1, 0, 1, alpha));

		if (m_slowTimeTrailCounter <= 0)
		{
			auto trail = TrailRenderer::Create(m_renderer);
			trail->SetBeginColor(Color(1, 0, 1, 0.5f));
			trail->SetEndColor(Color(1, 0, 1, 0));
			trail->SetDuration(0.1f);
			trail->SetTimeScaleMode(true);
			m_slowTimeTrailCounter = 0.02f;
		}
		else
		{
			m_slowTimeTrailCounter -= Time::GetDeltaTime();
		}
	}
	else
	{
		float alpha = m_renderer->GetColor().a;
		m_renderer->SetColor(Color(1, 1, 1, alpha));
	}
}

void Boss::OnDestroy()
{
	m_boxCollider->OnCollisionEnter -= Function<void, const CollisionResult&>(this, &Boss::OnCollisonEnter);
	m_boxCollider->OnCollisionExit -= Function<void, Collider*>(this, &Boss::OnCollisonExit);
	m_boxCollider->OnCollisionStay -= Function<void, const CollisionResult&>(this, &Boss::OnCollisonStay);
	m_boxCollider->OnTriggerEnter -= Function<void, Collider*>(this, &Boss::OnTriggerEnter);
	m_boxCollider->OnTriggerExit -= Function<void, Collider*>(this, &Boss::OnTriggerExit);
	m_boxCollider->OnTriggerStay -= Function<void, Collider*>(this, &Boss::OnTriggerStay);

	m_circleCollider->OnCollisionEnter -= Function<void, const CollisionResult&>(this, &Boss::OnCollisonEnter);
	m_circleCollider->OnCollisionExit -= Function<void, Collider*>(this, &Boss::OnCollisonExit);
	m_circleCollider->OnCollisionStay -= Function<void, const CollisionResult&>(this, &Boss::OnCollisonStay);
	m_circleCollider->OnTriggerEnter -= Function<void, Collider*>(this, &Boss::OnTriggerEnter);
	m_circleCollider->OnTriggerExit -= Function<void, Collider*>(this, &Boss::OnTriggerExit);
	m_circleCollider->OnTriggerStay -= Function<void, Collider*>(this, &Boss::OnTriggerStay);
}

bool Boss::TakeDamage(Vec2 velocity, Component* damageObject, bool canBlock)
{
	return Damage(velocity, damageObject, canBlock);
}

void Boss::SetDirection(float hd)
{
	m_rendererParentObject->GetTransform()->SetScale(Vec2(hd, 1));
}

float Boss::GetDirection() const
{
	return 	m_rendererParentObject->GetTransform()->GetScale().x;
}

bool Boss::GroundCheck(RaycastResult* pResult, float length) const
{
	Vec2 beg = m_circleCollider->GetTransform()->GetPosition();
	Vec2 end = beg + Vec2::down() * (m_circleCollider->GetRadius() + length);
	return Physics::Raycast(beg, end, pResult, LAYER_GROUND | LAYER_PLATFORM);
}

bool Boss::GroundCheck(float length) const
{
	RaycastResult temp;
	return GroundCheck(&temp, length);
}

void Boss::DetachFromGround()
{
	m_body->SetPosition(m_body->GetTransform()->GetPosition() + Vec2::up() * 10);
	m_ground = false;
}

bool Boss::HasGround() const
{
	return m_ground;
}

void Boss::SetHeight(float height)
{
	float radius = m_circleCollider->GetRadius();
	float boxHeight = height - radius;

	m_boxCollider->SetHeight(boxHeight);
	m_circleCollider->GetTransform()->SetLocalPosition(Vec2::down() * boxHeight * 0.5f);
}

void Boss::SetWidth(float width)
{
	m_boxCollider->SetWidth(width);
	m_circleCollider->SetRadius(width * 0.5f);
}

float Boss::GetHeight() const
{
	return m_boxCollider->GetHeight() + m_circleCollider->GetRadius();
}

float Boss::GetWidth() const
{
	return m_boxCollider->GetWidth();
}

Vec2 Boss::GetBottom() const
{
	Vec2 center = GetTransform()->GetPosition();
	Vec2 bottom = center + Vec2::down() * (m_boxCollider->GetHeight() * 0.5f + m_circleCollider->GetRadius());
	return bottom;
}

void Boss::SetSlowModeColorChange(bool enable)
{
	m_slowModeColorChange = enable;
}
