#include "stdafx.h"
#include "KitchenKnife.h"
#include "GameLayer.h"
#include "Door.h"
#include "SoundMgr.h"
#include "BreakablePlatform.h"

void KitchenKnife::Awake()
{
    ThrowObject::Awake();

    m_canCrashWithBullet = true;
    m_canReflectByBullet = true;
    m_useBreakEffect = false;
    m_door = nullptr;

    m_body->SetContinuousMode(true);
}

void KitchenKnife::Update()
{
    ThrowObject::Update();

    Check();
}

void KitchenKnife::AfterRender()
{
    Check();
}

void KitchenKnife::OnBreak(const Vec2& position, const Vec2& velocity, BreakType breakType)
{
    CSoundMgr::Get_Instance()->PlaySound(L"sound_knife_wall.wav", CSoundMgr::CHANNELID::THROW_OBJECT);

    if (breakType != BreakType::STAGE) return;

    Vec2 rayBeg = position - m_trigger->GetRadius() * velocity.normalized() * 2;
    Vec2 rayEnd = position + m_trigger->GetRadius() * velocity.normalized() * 2;

    Vec2 normal = velocity.normalized();
    Vec2 point = position - normal * m_trigger->GetRadius() * 2;

    Door* door = nullptr;
    BreakablePlatform* platform = nullptr;

    auto HorizontalDir = [](const Vec2& velocity)
    {
        if (velocity.x < 0) return Vec2::left();
        else return Vec2::right();
    };
    auto VerticalDir = [](const Vec2& velocity)
    {
        if (velocity.y < 0) return Vec2::down();
        else return Vec2::up();
    };
    auto Raycast = [](const Vec2& beg, const Vec2& end, RaycastResult* res)
    {
        return Physics::RaycastStraight(
            beg,
            end,
            res, LAYER_GROUND | LAYER_DOOR | LAYER_BREAKABLE_PLATFORM);
    };
    auto SelectDoorOrPlatform = [&](RaycastResult* res)
    {
        door = res->collider->GetBody()->GetComponent<Door>();
        platform = res->collider->GetBody()->GetComponent<BreakablePlatform>();
    };

    RaycastResult res;
    if (Raycast(
        rayBeg, 
        rayEnd, 
        &res))
    {
        normal = res.normal;
        point = res.point + normal * m_trigger->GetRadius();
        SelectDoorOrPlatform(&res);
    }
    else if (Raycast(
        position - HorizontalDir(velocity) * m_trigger->GetRadius() * 4,
        position + HorizontalDir(velocity) * m_trigger->GetRadius() * 4,
        &res))
    {
        normal = res.normal;
        point = res.point + normal * m_trigger->GetRadius();
        SelectDoorOrPlatform(&res);
    }
    else if (Raycast(
        position - VerticalDir(velocity) * m_trigger->GetRadius() * 4, 
        position + VerticalDir(velocity) * m_trigger->GetRadius() * 4,
        &res))
    {
        normal = res.normal;
        point = res.point + normal * m_trigger->GetRadius();
        SelectDoorOrPlatform(&res);
    }

    auto o = new GameObject(point, Vec2::Angle180(normal) - 90);
    KitchenKnife* newKnife = o->AddComponent<KitchenKnife>();
    newKnife->DestroyDustEffect();
    newKnife->m_door = door;
    newKnife->m_platform = platform;
}

void KitchenKnife::OnPutIn()
{
    m_door = nullptr;
}

wstring KitchenKnife::SpriteKey()
{
    return L"../Texture/object/used/throw/spr_butcher_knife_0.png";
}

void KitchenKnife::Check()
{
    if (m_door)
    {
        if (m_door->IsShouldDestroy())
        {
            DestroyDustEffect();
            GetGameObject()->Destroy();
            m_door = nullptr;
        }
        else if (m_door->IsOpen())
        {
            DestroyDustEffect();
            GetGameObject()->Destroy();
            m_door = nullptr;
        }
    }
    if (m_platform)
    {
        if (m_platform->IsShouldDestroy())
        {
            DestroyDustEffect();
            GetGameObject()->Destroy();
            m_platform = nullptr;
        }
        else if (m_platform->IsShouldBreak())
        {
            DestroyDustEffect();
            GetGameObject()->Destroy();
            m_platform = nullptr;
        }
    }
}
