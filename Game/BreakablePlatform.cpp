#include "stdafx.h"
#include "BreakablePlatform.h"
#include "GameLayer.h"
#include "GameRenderOrder.h"
#include "GameTag.h"
#include "DebugDraw.h"
#include "Piece.h"
#include "SoundMgr.h"

void BreakablePlatform::Awake()
{
	GetGameObject()->SetTag(TAG_BREAKABLE_PLATFORM);

	m_spr = new Sprite(L"../Texture/object/used/platform/spr_platform.png");
	m_renderer = AddComponent<SpriteRenderer>();
	m_renderer->SetRenderOrder(RENDER_ORDER_PLATFORM);
	m_renderer->SetSprite(m_spr);

	m_body = AddComponent<Body>();
	m_body->SetType(BodyType::Static);
	m_body->SetLayerIndex(LAYERINDEX_BREAKABLE_PLATFORM);

	auto colObj = GameObject::CreateChild(GetGameObject());
	colObj->GetTransform()->SetLocalPosition(Vec2::down() * 2.5f);
	m_collider = colObj->AddComponent<BoxCollider>();
	m_collider->SetWidth(32);
	m_collider->SetHeight(2);
	m_collider->DrawDebug = DRAW_OBJECT;
}

void BreakablePlatform::Update()
{
	if (m_destroyNextFrame)
	{
		GetGameObject()->Destroy();
		return;
	}
}

void BreakablePlatform::OnDestroy()
{
	SAFE_DELETE(m_spr);
}

void BreakablePlatform::BreakPlatform(const Vec2& velocity)
{
	m_destroyNextFrame = true;
	m_body->SetEnable(false);
	m_collider->SetEnable(false);

	auto left = new GameObject(GetTransform()->GetPosition() + Vec2::left() * 8);
	auto leftPiece = left->AddComponent<Piece>();
	leftPiece->SetSprite(L"../Texture/object/used/platform/spr_platform_broke_left.png");
	leftPiece->SetVelocity(velocity);

	auto right = new GameObject(GetTransform()->GetPosition() + Vec2::right() * 8);
	auto rightPiece = right->AddComponent<Piece>();
	rightPiece->SetSprite(L"../Texture/object/used/platform/spr_platform_broke_left.png");
	rightPiece->SetVelocity(velocity);

	RaycastResult res;
	Vec2 center = m_collider->GetTransform()->GetPosition();
	Vec2 leftpos = center + Vec2::left() * m_collider->GetWidth() * 2;
	Vec2 rightpos = center + Vec2::right() * m_collider->GetWidth() * 2;
	if (Physics::RaycastStraight(center, leftpos, &res, LAYER_BREAKABLE_PLATFORM))
	{
		BreakablePlatform* p = res.collider->GetBody()->GetComponent<BreakablePlatform>();
		p->BreakPlatform(velocity);
	}
	if (Physics::RaycastStraight(center, rightpos, &res, LAYER_BREAKABLE_PLATFORM))
	{
		BreakablePlatform* p = res.collider->GetBody()->GetComponent<BreakablePlatform>();
		p->BreakPlatform(velocity);
	}

	CSoundMgr::Get_Instance()->PlaySound(L"sound_object_floorbreak_wood_01.wav", CSoundMgr::CHANNELID::ENVIRONMENT);
}

bool BreakablePlatform::IsShouldBreak() const
{
	return m_destroyNextFrame;
}
