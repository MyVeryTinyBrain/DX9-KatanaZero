#include "stdafx.h"
#include "PlatformerBody.h"
#include "GameLayer.h" 
#include "DebugDraw.h"

void PlatformerBody::Awake()
{
	m_fixedUpdateExecutionOrder = FIXED_UPDATE_ORDER_BODY + 1;
	MakeBody();
	MakeColliders();
	SetMaxGroundAngle(50);
	SetGroundLayers(LAYER_GROUND | LAYER_PLATFORM);
	SetRayLength(10.0f);
	SetFriction(1000.0f);
	SetDefaultGravityScale(1.0f);
	SetStickGravityScale(10.0f);
	SetPlatformLayerIndex(LAYERINDEX_PLATFORM);
	m_jumping = false;
	SetUnrealisticPhysics(true);
	SetStickOnGround(true);
	SetIgnoreRestitute(true);
	SetIgnoreSlope(true);
	SetUseFriction(true);
	m_throughPlatformOnce = false;
	SetTripleRay(true);
	SetThroughPlatform(false);

	m_body->SetContinuousMode(true);
}

void PlatformerBody::FixedUpdate()
{
	m_hasRayResult = RaycastCheck(&m_rayResult);
	//if (m_hasRayResult && 
	//	(m_rayResult.collider->GetLayer() & LAYER_PLATFORM) > 0 && 
	//	GetVeloicty().y > 0) 
	//	m_hasRayResult = false;

	ApplyFriction();
	IgnoreSlope();
	StickOnGround();

	if (m_throughPlatformOnce)
	{
		if (m_throughPlatformOnceElapsed > 0.1f || m_body->GetVelocity().y > 0)
		{
			m_throughPlatformOnce = false;
			m_throughPlatformOnceElapsed = 0;
			if (m_hasRayResult) m_jumping = false;
		}
		else
		{
			m_throughPlatformOnceElapsed += Time::GetFixedDeltaTime();
		}
	}

	if (!m_hasRayResult)
	{
		m_airTime += Time::GetFixedDeltaTime();
	}
}

void PlatformerBody::LateFixedUpdate()
{
	ThroughPlatform();
}

void PlatformerBody::Render()
{
	DrawRaycastCheck();
}

void PlatformerBody::OnDestroy()
{
	DetachBody(m_body);
	DetachCollider(m_circleCollider);
}

void PlatformerBody::OnDestroyBody(Object* body)
{
	DetachBody(static_cast<Body*>(body));
}

void PlatformerBody::OnDestroyCollider(Object* collider)
{
	DetachCollider(static_cast<Collider*>(collider));
}

void PlatformerBody::OnCollisionEnter(const CollisionResult& collision)
{
	if (collision.A->GetLayer() == LayerManager::GetLayer(m_platformLayerIndex))
	{
		m_body->SetVelocity(Vec2(m_body->GetVelocity().x, 0));
	}

	bool valid = AddGround(collision);
	if (valid)
	{
		IgnoreRestitute();
		m_jumping = false;
		m_throughPlatformOnce = false;
		m_throughPlatformOnceElapsed = 0;

		if (m_airTime > Time::GetFixedDeltaTime() * 2)
		{
			OnGround(collision.worldNormal, collision.worldPoints[0]);
		}
		m_airTime = 0;
	}
}

void PlatformerBody::OnCollisionExit(Collider* collider)
{
	RemoveGround(collider);
}

void PlatformerBody::OnCollisionStay(const CollisionResult& collision)
{
}

Body* PlatformerBody::GetBody() const
{
	return m_body;
}

BoxCollider* PlatformerBody::GetBoxCollider() const
{
	return m_boxCollider;
}

CircleCollider* PlatformerBody::GetCircleCollider() const
{
	return m_circleCollider;
}

void PlatformerBody::SetWidth(float width)
{
	// 사각형과 원의 너비를 설정합니다.

	float boxHeight = m_boxCollider->GetHeight();

	m_boxCollider->SetWidth(width);
	m_circleCollider->SetRadius(width * 0.5f + 0.0f);

	const Vec2& boxCenter = m_boxCollider->GetTransform()->GetPosition();
	m_circleCollider->GetTransform()->SetPosition(boxCenter + Vec2::down() * boxHeight * 0.5f);
}

void PlatformerBody::SetHeight(float height)
{
	// 사각형의 높이를 설정합니다.

	// 반 너비보다 작게 설정된 경우 너비를 높이의 2배로 설정합니다.
	// 이것은 언제나 원의 위쪽 절반이 사각형에 포함되도록 합니다.
	float boxWidth = m_boxCollider->GetWidth();
	float boxhw = boxWidth * 0.5f;
	if (height < boxhw)
	{
		SetWidth(height * 2);
		boxWidth = boxhw;
		boxhw = boxWidth * 0.5f;
	}

	m_boxCollider->SetHeight(height);

	const Vec2& boxCenter = m_boxCollider->GetTransform()->GetPosition();
	m_circleCollider->GetTransform()->SetPosition(boxCenter + Vec2::down() * height * 0.5f);
}

void PlatformerBody::SetFullHeight(float height)
{
	// 원과 사각형이 겹친 도형 자체의 높이를 설정합니다.

	float boxWidth = m_boxCollider->GetWidth();
	float boxhw = boxWidth * 0.5f;
	SetHeight(height - boxhw);
}

float PlatformerBody::GetWidth() const
{
	return m_boxCollider->GetWidth();
}

float PlatformerBody::GetHeight() const
{
	return m_boxCollider->GetHeight();
}

float PlatformerBody::GetFullHeight() const
{
	return m_boxCollider->GetHeight() + m_circleCollider->GetRadius();
}

float PlatformerBody::GetCenterToBottom() const
{
	return m_boxCollider->GetHeight() * 0.5f + m_circleCollider->GetRadius();
}

Vec2 PlatformerBody::GetBottom() const
{
	return m_circleCollider->GetTransform()->GetPosition() + Vec2::down() * m_circleCollider->GetRadius();
}

void PlatformerBody::SetBottom(Vec2 position)
{
	Vec2 center = position + Vec2::up() * GetCenterToBottom();
	m_body->SetPosition(center);
}

void PlatformerBody::SetMaxGroundAngle(float angle)
{
	m_maxGroundAngle = angle + 0.1f;
	if (m_maxGroundAngle <= 0.00f) m_maxGroundAngle = 0.1f;
	if (m_maxGroundAngle >= 90.0f) m_maxGroundAngle = 90.0f;
}

void PlatformerBody::SetGroundLayers(layer_t layers)
{
	m_groundLayers = layers;
}

void PlatformerBody::SetRayLength(float length)
{
	m_rayLength = length;
	if (m_rayLength < 1.0f)
		m_rayLength = 1.0f;
}

void PlatformerBody::SetFriction(float friction)
{
	m_friction = friction;
}

void PlatformerBody::SetDefaultGravityScale(float scale)
{
	m_defaultGravityScale = scale;
}

void PlatformerBody::SetStickGravityScale(float scale)
{
	m_stickGravityScale = scale;
}

void PlatformerBody::SetPlatformLayerIndex(unsigned int layerIndex)
{
	m_platformLayerIndex = layerIndex;
}

void PlatformerBody::SetUnrealisticPhysics(bool enable)
{
	m_unrealisticPhysicsMode = enable;
}

void PlatformerBody::SetStickOnGround(bool enable)
{
	m_stickOnGround = enable;
}

void PlatformerBody::SetIgnoreRestitute(bool enable)
{
	m_ignoreRestitute = enable;
}

void PlatformerBody::SetIgnoreSlope(bool enable)
{
	m_ignoreSlope = enable;
}

void PlatformerBody::SetUseFriction(bool enable)
{
	m_useFriction = enable;
}

void PlatformerBody::SetTripleRay(bool enable)
{
	m_tripleRay = enable;
}

void PlatformerBody::AccelerateTo(const Vec2& direction, float speed, float increase)
{
	m_body->AccelerateTo(direction, speed, increase);
}

void PlatformerBody::SetHorizontalVelocity(float hv)
{
	if (HasGround())
	{
		Quat rotation = Quat::FromToRotation(Vec2::up(), m_rayResult.normal);
		Vec4 velocity = rotation.ToMatrix() * Vec4(hv, 0, 0, 0);
		Vec2 rotatedVelocity = rotation * Vec3(hv, 0, 0);
		m_body->SetVelocity(rotatedVelocity);
	}
	else
	{
		Vec2 velocity = m_body->GetVelocity();
		velocity.x += hv * Time::GetFixedDeltaTime();
		m_body->SetVelocity(velocity);
	}
}

void PlatformerBody::SetJump(float vv)
{
	DetachFromGround();

	Vec2 position = GetTransform()->GetPosition();
	position.y += m_rayLength;
	GetTransform()->SetPosition(position);

	Vec2 velocity = m_body->GetVelocity();
	velocity.y = vv;
	m_body->SetVelocity(velocity);

	m_jumping = true;
}

void PlatformerBody::SetJump(const Vec2& v)
{
	DetachFromGround();

	Vec2 position = GetTransform()->GetPosition();
	position.y += m_rayLength;
	GetTransform()->SetPosition(position);

	m_body->SetVelocity(v);

	m_jumping = true;
}

void PlatformerBody::CancelJumpState()
{
	m_jumping = false;
}

void PlatformerBody::SetVelocity(const Vec2& v)
{
	m_body->SetVelocity(v);
}

void PlatformerBody::AddVelocity(const Vec2& v)
{
	m_body->AddVelocity(v);
}

Vec2 PlatformerBody::GetVeloicty() const
{
	return m_body->GetVelocity();
}

void PlatformerBody::ThroughPlatformOnce()
{
	if (!m_hasRayResult) return;
	if (m_rayResult.collider->GetLayer() != LayerManager::GetLayer(m_platformLayerIndex)) return;
	m_jumping = true;
	m_throughPlatformOnce = true;
	m_throughPlatformOnceElapsed = 0;
}

bool PlatformerBody::HasGround() const
{
	return (m_grounds.size() > 0) && m_hasRayResult;
}

bool PlatformerBody::HasCollision() const
{
	return (m_grounds.size() > 0);
}

bool PlatformerBody::HasRayHit() const
{
	return m_hasRayResult;
}

RaycastResult PlatformerBody::GetRaycastResult() const
{
	return m_rayResult;
}

bool PlatformerBody::IsJumping() const
{
	return m_jumping;
}

void PlatformerBody::SetThroughPlatform(bool enable)
{
	m_throughPlatform = enable;
}

bool PlatformerBody::GetThroughPlatform() const
{
	return m_throughPlatform;
}

void PlatformerBody::MakeBody()
{
	m_body = AddComponent<Body>();
	m_body->SetFixedRotationMode(true);
	m_body->SetLayerIndex(LAYERINDEX_CHARACTER);
	AttachBody(m_body);
}

void PlatformerBody::MakeColliders()
{
	GameObject* boxColliderObject = GameObject::CreateChild(GetGameObject());
	m_boxCollider = boxColliderObject->AddComponent<BoxCollider>();
	m_boxCollider->DrawDebug = DRAW_CHARACTER;

	GameObject* circleColliderObject = GameObject::CreateChild(GetGameObject());
	circleColliderObject->GetTransform()->SetLocalPosition(Vec2::down() * m_boxCollider->GetHeight() * 0.5f);
	m_circleCollider = circleColliderObject->AddComponent<CircleCollider>();
	m_circleCollider->DrawDebug = DRAW_CHARACTER;

	SetWidth(12);
	SetHeight(22);

	AttachCollider(m_circleCollider);
}

void PlatformerBody::AttachBody(Body* body)
{
	m_body->OnDestoryCallback += Function<void, Object*>(this, &PlatformerBody::OnDestroyBody);
}

void PlatformerBody::AttachCollider(Collider* collider)
{
	collider->OnDestoryCallback += Function<void, Object*>(this, &PlatformerBody::OnDestroyCollider);
	collider->OnCollisionEnter += Function<void, const CollisionResult&>(this, &PlatformerBody::OnCollisionEnter);
	collider->OnCollisionExit += Function<void, Collider*>(this, &PlatformerBody::OnCollisionExit);
	collider->OnCollisionStay += Function<void, const CollisionResult&>(this, &PlatformerBody::OnCollisionStay);
}

void PlatformerBody::DetachBody(Body* body)
{
	if (!body) return;
	if (body != m_body) return;
	m_body->OnDestoryCallback -= Function<void, Object*>(this, &PlatformerBody::OnDestroyBody);
	m_body = nullptr;
}

void PlatformerBody::DetachCollider(Collider* collider)
{
	if (!collider) return;
	if (collider != m_circleCollider) return;
	collider->OnDestoryCallback -= Function<void, Object*>(this, &PlatformerBody::OnDestroyCollider);
	collider->OnCollisionEnter -= Function<void, const CollisionResult&>(this, &PlatformerBody::OnCollisionEnter);
	collider->OnCollisionExit -= Function<void, Collider*>(this, &PlatformerBody::OnCollisionExit);
	collider->OnCollisionStay -= Function<void, const CollisionResult&>(this, &PlatformerBody::OnCollisionStay);
	collider = nullptr;
}

bool PlatformerBody::AddGround(const CollisionResult& collision)
{
	// 바닥 레이어만 인식합니다.
	if ((collision.A->GetLayer() & m_groundLayers) == 0) return false;

	// 정적 바디만 인식합니다.
	if (collision.A->GetBody()->GetType() != BodyType::Static) return false;

	// 충돌한 콜라이더의 법선과 역중력 사이의 각도가 지정한 각도 이상이라면 종료합니다.
	float angle = Vec2::Angle(collision.worldNormal, -Physics::GetGravity());
	if (angle > m_maxGroundAngle) return false;

	CollidedGround ground;
	ground.collider = collision.A;
	ground.angle = angle;
	ground.normal = collision.worldNormal;
	m_grounds.push_back(ground);

	// 작은 각도 순으로 정렬합니다.
	std::sort(m_grounds.begin(), m_grounds.end(),
		[&](CollidedGround& lhs, CollidedGround& rhs)
		{
			return lhs.angle < rhs.angle;
		});

	return true;
}

void PlatformerBody::RemoveGround(Collider* collider)
{
	auto it = std::find_if(m_grounds.begin(), m_grounds.end(),
		[&](CollidedGround& ground)
		{
			return ground.collider == collider;
		});
	if (it != m_grounds.end()) m_grounds.erase(it);
	else return;

	// 작은 각도 순으로 정렬합니다.
	std::sort(m_grounds.begin(), m_grounds.end(),
		[&](CollidedGround& lhs, CollidedGround& rhs)
		{
			return lhs.angle < rhs.angle;
		});
}

bool PlatformerBody::RaycastCheck(RaycastResult* pResult) const
{
	// 3번 레이캐스트를 합니다.
	// 단 한번이라도 성공하면 True와 함께 pResult에 결과를 저장합니다.

	// 콜라이더의 중심입니다.
	Vec2 begin = m_circleCollider->GetTransform()->GetPosition();

	// begin에서 중력 방향으로 rayLength 만큼 내린 위치입니다.
	Vec2 end = begin + Physics::GetGravity().normalized() * m_rayLength;

	bool hasHit = false;

	// 중심에서 레이캐스트
	Vec2 moveToBottom = Vec2::down() * m_circleCollider->GetRadius();
	if (Physics::Raycast(begin + moveToBottom, end + moveToBottom, pResult, m_groundLayers))
	{
		hasHit = true;
		if (!m_tripleRay)
		{
			return true;
		}
		else if (pResult->collider->GetLayer() != LayerManager::GetLayer(m_platformLayerIndex))
		{
			return true;
		}
	}

	if (!m_tripleRay)
	{
		return hasHit;
	}

	// 왼쪽에서 레이캐스트
	Vec2 moveToLeft = Vec2::left() * m_circleCollider->GetRadius() * 0.5f;
	if (Physics::Raycast(begin + moveToLeft, end + moveToLeft, pResult, m_groundLayers))
	{
		hasHit = true;
		if (pResult->collider->GetLayer() != LayerManager::GetLayer(m_platformLayerIndex))
		{
			return true;
		}
	}

	// 오른쪽에서 레이캐스트
	Vec2 moveToRight = Vec2::right() * m_circleCollider->GetRadius() * 0.5f;
	if (Physics::Raycast(begin + moveToRight, end + moveToRight, pResult, m_groundLayers))
	{
		hasHit = true;
		if (pResult->collider->GetLayer() != LayerManager::GetLayer(m_platformLayerIndex))
		{
			return true;
		}
	}

	return hasHit;
}

void PlatformerBody::DrawRaycastCheck()
{
#ifdef _DEBUG
	if (!DRAW_RAY) return;
	LineDevice* line = GraphicDevice::GetLineDevice();
	Draw2DLineParameters p;
	p.color = Color(0, 1, 0, 1);
	{	// Ground check line
		Vec2 begin = m_circleCollider->GetTransform()->GetPosition();
		Vec2 end = begin + Physics::GetGravity().normalized() * m_rayLength;
		Vec2 moveToBottom = Vec2::down() * m_circleCollider->GetRadius();
		Vec2 moveToLeft = Vec2::left() * m_circleCollider->GetRadius() * 0.5f;
		Vec2 moveToRight = Vec2::right() * m_circleCollider->GetRadius() * 0.5f;
		line->Draw2DLine(begin + moveToBottom, end + moveToBottom, &p);
		line->Draw2DLine(begin + moveToLeft, end + moveToLeft, &p);
		line->Draw2DLine(begin + moveToRight, end + moveToRight, &p);
	}
	{	// Platform Check line
		Vec2 begin = m_boxCollider->GetTransform()->GetPosition() + Vec2::up() * m_boxCollider->GetHeight() * 0.5f;
		Vec2 end = m_boxCollider->GetTransform()->GetPosition() + Vec2::down() * (m_boxCollider->GetHeight() * 0.5f + m_circleCollider->GetRadius() * 0.5f);
		line->Draw2DLine(begin, end, &p);
	}
#endif
}

void PlatformerBody::DetachFromGround()
{
	m_grounds.clear();
	m_hasRayResult = false;
}

void PlatformerBody::IgnoreRestitute()
{
	if (!m_ignoreRestitute || !m_unrealisticPhysicsMode)
	{
		return;
	}

	m_body->SetVelocity(Vec2::zero());
}

void PlatformerBody::IgnoreSlope()
{
	if (!m_ignoreSlope /*|| !m_unrealisticPhysicsMode*/)
	{
		m_body->SetGravityMode(true);
		return;
	}

	if (!HasRayHit() && HasCollision())
	{
		m_body->SetGravityMode(true);
		return;
	}

	m_body->SetGravityMode(HasCollision() ? false : true);
}

void PlatformerBody::StickOnGround()
{
	if (!m_stickOnGround || !m_unrealisticPhysicsMode)
	{
		m_body->SetGravityScale(m_defaultGravityScale);
		return;
	}

	float gravityScale = m_defaultGravityScale;
	if (!m_jumping && HasRayHit() && !HasCollision())
	{
		if (m_rayResult.collider->GetLayer() == LayerManager::GetLayer(m_platformLayerIndex))
		{
			//if (m_body->GetVelocity().y < 0) gravityScale = m_stickGravityScale;
			//else gravityScale = m_defaultGravityScale;

			//gravityScale = m_stickGravityScale;

			gravityScale = m_defaultGravityScale;
		}
		else
		{
			gravityScale = m_stickGravityScale;
		}
	}
	m_body->SetGravityScale(gravityScale);
}

void PlatformerBody::ApplyFriction()
{
	if (!m_useFriction/* || !m_unrealisticPhysicsMode*/)
	{
		return;
	}
	if (!HasGround()) return;

	Vec2 velocity = m_body->GetVelocity();
	float m_linearDamping = m_friction;
	velocity *= 1.0f / (1.0f + m_linearDamping * Time::GetFixedDeltaTime());
	m_body->SetVelocity(velocity);

}

void PlatformerBody::ThroughPlatform()
{
	const Vec2& v = m_body->GetVelocity();
	if (m_throughPlatformOnce || m_throughPlatform || v.y > 10.0f)
	{
		m_boxCollider->SetIgnoreLayerIndex(m_platformLayerIndex, true);
		m_circleCollider->SetIgnoreLayerIndex(m_platformLayerIndex, true);
	}
	else
	{
		/*
			 Begin
			┌─────┐
			│	  │
			│     │	<-- Box Collider
			│     │
			│     │
			│     │
			├─────┤
			│ End │ <-- Circle Collider
			└─────┘
		*/

		Vec2 begin = m_boxCollider->GetTransform()->GetPosition() + Vec2::up() * (m_boxCollider->GetHeight() * 0.5f);
		//Vec2 end = m_boxCollider->GetTransform()->GetPosition() + Vec2::down() * (m_boxCollider->GetHeight() * 0.5f + m_circleCollider->GetRadius() * 0.45f);
		Vec2 end = m_boxCollider->GetTransform()->GetPosition() + Vec2::down() * (m_boxCollider->GetHeight() * 0.5f + 1.0f);
		RaycastResult inside;
		if (Physics::Raycast(begin, end, &inside, LayerManager::GetLayer(m_platformLayerIndex)))
		{
			m_boxCollider->SetIgnoreLayerIndex(m_platformLayerIndex, true);
			m_circleCollider->SetIgnoreLayerIndex(m_platformLayerIndex, true);
		}
		else
		{
			m_boxCollider->SetIgnoreLayerIndex(m_platformLayerIndex, false);
			m_circleCollider->SetIgnoreLayerIndex(m_platformLayerIndex, false);
		}
	}
}
