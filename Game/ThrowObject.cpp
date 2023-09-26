#include "stdafx.h"
#include "ThrowObject.h"
#include "GameTag.h"
#include "GameLayer.h"
#include "StaticEffect.h"
#include "Enemy.h"
#include "TimeController.h"
#include "PlayerCamera.h"
#include "HitLineEffect.h"
#include "PlayerController.h"
#include "GameRenderOrder.h"
#include "DebugDraw.h"
#include "DownArrow.h"
#include "EnemyShieldCop.h"
#include "Piece.h"

#define PUT_IN_MIN_DISTANCE (50)

void ThrowObject::Awake()
{
    m_updateExecutionOrder = INT_MAX;
    GetGameObject()->SetTag(TAG_THROW_OBJECT);

	m_renderer = AddComponent<SpriteRenderer>();
    m_renderer->SetRenderOrder(RENDER_ORDER_THROW_OBJECT);
	m_sprite = new Sprite(SpriteKey());
    m_renderer->SetSprite(m_sprite);
	m_mode = ThrowObjectMode::DROPPED;

	m_body = AddComponent<Body>();
	m_body->SetGravityMode(false);
	m_body->SetLayerIndex(LAYERINDEX_TRIGGER);
    m_body->SetContinuousMode(true);
	m_trigger = AddComponent<CircleCollider>();
	m_trigger->SetTriggerMode(true);
    m_trigger->SetRadius(10);
    m_trigger->DrawDebug = DRAW_OBJECT;
    m_trigger->OnTriggerEnter += Function<void, Collider*>(this, &ThrowObject::OnTriggerEnter);
    m_body->SetEnable(false);
    m_trigger->SetEnable(false);

    m_destroyWhenNextFrame = false;
    m_damageToEnemy = true;
    m_canCrashWithBullet = true;
    m_canReflectByBullet = false;
    m_useBreakEffect = true;
    m_canOnlyBlockWithShield = true;

    auto o = GameObject::CreateChild(GetGameObject());
    m_arrow = o->AddComponent<DownArrow>();
}

void ThrowObject::Update()
{
    if (m_destroyWhenNextFrame)
    {
        GetGameObject()->Destroy();
        return;
    }

    if (m_mode == ThrowObjectMode::THROW)
    {
        float angle = GetTransform()->GetAngle();
        if (m_body->GetVelocity().x < 0)
            angle += Time::GetDeltaTime() * 360 * 6;
        else 
            angle -= Time::GetDeltaTime() * 360 * 6;
        GetTransform()->SetAngle(angle);

        m_throwingElapsed += Time::GetDeltaTime();
        if (m_throwingElapsed > 3.0f)
        {
            GetGameObject()->Destroy();
            return;
        }
    }

    PutIn();
}

void ThrowObject::OnDestroy()
{
	SAFE_DELETE(m_sprite);
}

void ThrowObject::OnTriggerEnter(Collider* collider)
{
    if (m_destroyWhenNextFrame) return;
    if (m_mode != ThrowObjectMode::THROW) return;

    Body* body = collider->GetBody();

    if (body->GetLayerIndex() == LAYERINDEX_GROUND ||
        body->GetLayerIndex() == LAYERINDEX_DOOR ||
        body->GetLayerIndex() == LAYERINDEX_BREAKABLE_PLATFORM)
	{
        Break(BreakType::STAGE);
	}
	else if (body->GetGameObject()->GetTag() == TAG_ENEMY)
	{
        Enemy* enemy = body->GetComponent<Enemy>();
        if (!enemy) return;
        if (enemy->IsHurt()) return;

        if (m_damageToEnemy)
        {
            bool canBlock = true;
            if (m_canOnlyBlockWithShield)
            {
                if (dynamic_cast<EnemyShieldCop*>(enemy)) canBlock = true;
                else canBlock = false;
            }

            bool dead = enemy->Damage(m_body->GetVelocity().normalized() * 400, this, canBlock);
            GetGameObject()->Destroy();

            TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
            PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);

            if (dead)
            {
                HitLineEffect::Create(body->GetTransform()->GetPosition(), m_body->GetVelocity().normalized());
            }
        }

        Break(BreakType::ENEMY);
	}
}

const wstring& ThrowObject::GetSpriteKey() const
{
    return m_sprite->GetTexture()->GetKey();
}

bool ThrowObject::CanCrashWithBullet() const
{
    return m_canCrashWithBullet;
}

void ThrowObject::Crash(const Vec2& velocity)
{
    m_canCrashWithBullet = false;

    if (m_canReflectByBullet)
    {
        auto effect = StaticEffect::Create(L"../Animation/effect/bulletreflect.txt", GetTransform()->GetPosition());
        m_body->SetVelocity(velocity);
    }
    else
    {
        Break(BreakType::BULLET);
    }
}

void ThrowObject::Throw(const Vec2& position, const Vec2& velocity)
{
    GetTransform()->SetScale(Vec2::one());
    GetGameObject()->SetEnable(true);
    m_body->SetEnable(true);
    m_trigger->SetEnable(true);
    m_renderer->SetRenderOrder(RENDER_ORDER_THROW_OBJECT_THROWING);

    GetTransform()->SetPosition(position);
    m_body->SetVelocity(velocity);
    m_body->UpdateInterpolation();
    m_mode = ThrowObjectMode::THROW;
}

void ThrowObject::MakePiecesWhenTrigger(const wstring& pieceSpriteKey)
{
    Vec2 beg = GetTransform()->GetPosition();
    Vec2 normal = -m_body->GetVelocity().normalized();
    Vec2 end = beg + normal * m_trigger->GetRadius();
    Vec2 point = beg;
    RaycastResult res;
    if (Physics::Raycast(end, beg, &res, LAYER_GROUND | LAYER_DOOR | LAYER_BREAKABLE_PLATFORM))
    {
        point = res.point;
        normal = res.normal;
    }

    float w = float(TextureManager::GetTexture(pieceSpriteKey)->GetImageInfo().Width);
    float h = float(TextureManager::GetTexture(pieceSpriteKey)->GetImageInfo().Height);
    float longSide = w;
    if (h > w) longSide = h;

    for (int i = 0; i < 10; ++i)
    {
        auto o = new GameObject(point + normal * longSide * 0.5f);
        auto piece = o->AddComponent<Piece>();
        piece->SetSprite(pieceSpriteKey);
        piece->SetPower(normal, 300, 360);
    }
}

void ThrowObject::PutIn()
{
    if (m_mode != ThrowObjectMode::DROPPED) return;
    if (!PlayerController::GetInstance()->CanPutInThrowObject()) return;

    Vec2 playerPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
    Vec2 curPos = GetTransform()->GetPosition();
    Vec2 relVec = playerPos - curPos;
    float distance = relVec.magnitude();

    if (distance < PUT_IN_MIN_DISTANCE && Input::GetKeyDown(Key::RightMouse))
    {
        PlayerController::GetInstance()->PutInThrowObject(this);
        GetGameObject()->SetEnable(false);
        m_arrow->GetGameObject()->SetEnable(false);
        OnPutIn();
    }
}

void ThrowObject::Break(BreakType breakType)
{
    if (m_useBreakEffect)
        DestroyDustEffect();
    m_destroyWhenNextFrame = true;

    OnBreak(GetTransform()->GetPosition(), m_body->GetVelocity(), breakType);
}

void ThrowObject::DestroyDustEffect()
{
    constexpr int max_smoke = 20;
    for (int i = 0; i < max_smoke; ++i)
    {
        float percent = float(i) / float(max_smoke);
        float angle = percent * 360;
        float scale = 0.5f + float(rand() % 100) * 0.01f;
        wstring key;
        int key_state = rand() % 3;
        if (key_state == 0) key = L"../Animation/effect/gunsmoke0.txt";
        else if (key_state == 1) key = L"../Animation/effect/gunsmoke1.txt";
        else if (key_state == 2) key = L"../Animation/effect/gunsmoke2.txt";
        StaticEffect::Create(
            key,
            GetTransform()->GetPosition() + Vec2::Direction(angle * DEG2RAD) * float(rand() % 30),
            angle,
            Vec2::one() * scale,
            Vec2::zero(), Vec2::zero(),
            false,
            true)->SetSpeed(float(rand() % 100) * 0.01f + 1.0f);;
    }
}
