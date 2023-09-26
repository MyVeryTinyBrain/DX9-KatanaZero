#include "stdafx.h"
#include "Headhunter.h"
#include "HeadHunterAnimator.h"
#include "PlatformerBody.h"
#include "StaticEffect.h"
#include "BeamLock.h"
#include "GameLayer.h"
#include "PlayerController.h"
#include "DamageZone.h"
#include "Granade.h"
#include "TrailRenderer.h"
#include "GameTag.h"
#include "GameRenderOrder.h"
#include "TimeController.h"
#include "PlayerCamera.h"
#include "Bullet.h"
#include "Trigger.h"
#include "HeadhunterBeamPattern.h"
#include "RecordableRenderer.h"
#include "HeadhunterTurretPattern.h"
#include "SoundMgr.h"
#include "HeadhunterDie.h"

#define MAX_HP (4)

void Headhunter::Awake()
{
    Boss::Awake();

    m_rendererParentObject->GetTransform()->SetLocalPosition(Vec2::down() * 26);

    m_animator = AddComponent<HeadHunterAnimator>();
    m_animator->OnPlayEndHurtFlyLand += Function<void>(this, &Headhunter::OnPlayEndHurtFlyLand);
    m_animator->OnPlayJump += Function<void>(this, &Headhunter::OnPlayJump);
    m_animator->OnPlayEndRifleReady += Function<void>(this, &Headhunter::OnPlayEndRifleReady);
    m_animator->OnPlayEndGranadeReady += Function<void>(this, &Headhunter::OnPlayEndGranadeReady);
    m_animator->OnPlayEndGranadeShoot += Function<void>(this, &Headhunter::OnPlayEndGranadeShoot);
    m_animator->OnGranadeShootFrame += Function<void>(this, &Headhunter::OnGranadeShootFrame);
    m_animator->OnPlayEndDashReady += Function<void>(this, &Headhunter::OnPlayEndDashReady);
    m_animator->OnPlayEndRoll += Function<void>(this, &Headhunter::OnPlayEndRoll);

    m_wait = 0.4f;

    m_wallGrabDelay = 0.0f;
    m_wallGrabCounter = 0;
    m_granadeShootCount = 0;
    m_beamLockTime = 0.35f;
    m_beamDuration = 0.13f;
    m_dodgeDistance = 110.0f;

    m_sprDashTrail = new Sprite(L"../Texture/boss/headhunter/dash_begin/spr_headhunter_dashtrail_effect.png");

    m_leftWallX = Trigger::FindWithName(L"LeftWall")->GetTransform()->GetPosition().x;
    m_rightWallX = Trigger::FindWithName(L"RightWall")->GetTransform()->GetPosition().x;
    m_floorY = Trigger::FindWithName(L"Floor")->GetTransform()->GetPosition().y;
    m_wallGrabY = Trigger::FindWithName(L"WallGrabY")->GetTransform()->GetPosition().y;
    m_air = Trigger::FindWithName(L"AirY")->GetTransform()->GetPosition();

    m_hp = MAX_HP;
}

void Headhunter::Start()
{
}

void Headhunter::BeforeFixedStep()
{
    // 중력을 없애는 플래그입니다.
    bool ignoreGravity = 
        m_animator->IsPlayingWallGrab() || 
        m_animator->IsPlayingDashReady() ||
        m_animator->IsPlayingFastDashReady();
    // 중력을 설정합니다.
    m_body->SetGravityMode(!ignoreGravity);
}

void Headhunter::FixedUpdate()
{
    // 접지 상태를 설정합니다.
    bool g = HasGround();
    m_animator->SetGround(HasGround());

    Boss::FixedUpdate();

    if (m_hp <= 0) return;

    // 구르기 중에는 마찰을 줄입니다.
    m_groundDamping = (m_animator->IsPlayingRoll() ? 5.0f : 15.0f);

    if (m_animator->IsPlayingWallGrab())
    {
        if (m_wallGrabCounter < m_wallGrabDelay)
        {
            m_wallGrabCounter += Time::GetFixedDeltaTime();
        }
        else
        {
            // 일정 시간동안 벽을 잡은 후에는 
            // 벽의 반대 방향으로 점프합니다.

            m_wallGrabCounter = m_wallGrabDelay;
            m_body->SetVelocity(Vec2(-GetDirection(), 1.1f).normalized() * 600);
            DetachFromGround();
            m_animator->SetGround(false);
            m_animator->PlayWallJump();
            m_bulletStep = 0;
            OnWallJump();
        }
    }
    if (m_animator->IsPlayingJumpLoop() && RaycastToWall(-GetDirection()))
    {
        // 점프 중 일때 뒤쪽에 벽에 있다면
        // 벽을 잡습니다.
        // 이때 속도를 없애고 벽을 바라봅니다.

        m_animator->PlayWallGrab();
        m_body->SetVelocity(Vec2::zero());
        SetDirection(-GetDirection());
        m_wallGrabCounter = 0;
        OnWallGrab();
    }
}

void Headhunter::Update()
{
    Boss::Update();

    if (m_hp <= 0) return;

    if (m_animator->IsPlayingAimming())
    {
        // 조준 중일때 각도를 설정합니다.

        Vec2 playerPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
        Vec2 curPos = GetTransform()->GetPosition();
        Vec2 relVec = playerPos - curPos;
        float hd = relVec.x < 0 ? -1.0f : +1.0f;
        relVec.x *= hd;
        float angle = Vec2::Angle180(relVec);
        m_animator->SetAimAngle(angle);

        // 플레이어가 뒤로 간 경우에 조준을 종료합니다.
        if (GetDirection() != hd)
        {
            m_animator->PlayStopAimming();
        }
    }

    constexpr float bulletMax = 30;
    constexpr float stepDelta = 1.0f / bulletMax;
    constexpr float angleRange = 180.0f;
    float wallJumpPercent;
    if (m_animator->IsPlayingWallJump() && (wallJumpPercent = m_animator->GetWallJumpPercent()) > 0)
    {
        int step = int(wallJumpPercent / stepDelta);
        while (step >= m_bulletStep)
        {
            float stepPercent = m_bulletStep / bulletMax;
            float begAngle = 270 + (GetDirection() > 0 ? -90.0f : +90.0f);
            float adjustAngle = stepPercent * angleRange * GetDirection();
            float angle = begAngle + adjustAngle;
            Bullet::Create(
                GetTransform()->GetPosition() + Vec2::down() * 30,
                Vec2::Direction(angle * DEG2RAD) ,
                1200,
                true);
            ++m_bulletStep;
        }

        if (m_readyToPlayGatlingSound)
        {
            m_readyToPlayGatlingSound = false;
            CSoundMgr::Get_Instance()->PlaySound(L"sound_boss_huntress_gatling_01.wav", CSoundMgr::CHANNELID::BOSS_EFFECT_1);
        }
    }

    if (m_animator->IsPlayingAnimationAboutGranade() || m_animator->IsPlayingJumpLand())
    {
        if (GetRelativeToPlayer().magnitude() < m_dodgeDistance)
        {
            Dodge();
        }
    }
    if (m_animator->IsPlayingAnimationAboutAimming())
    {
        if (GetRelativeToPlayer().magnitude() < m_dodgeDistance)
        {
            CancelBeamLock();
            Dodge();
        }
    }

    if (m_animator->IsPlayingAimming())
    {
        if (m_beamFireCounter > 0)
        {
            m_beamFireCounter -= Time::GetDeltaTime();
        }
        else
        {
            m_beamFireCounter = m_beamLockTime + m_beamDuration + 0.5f;
            CreateBeamLockWhenAimming();
        }

        if (m_beamLock)
        {
            //float curAngle = m_beamLock->GetTransform()->GetAngle();
            //float targetAngle = GetToPlayerAngle(m_beamLock->GetTransform()->GetPosition());
            //float lerpAngle = curAngle + (targetAngle - curAngle) * Time::GetDeltaTime();
            //m_beamLock->GetTransform()->SetAngle(lerpAngle);

            Vec2 curDir = m_beamLock->GetTransform()->GetRight();
            Vec2 targetDir = GetRelativeToPlayer().normalized();
            Quat begQ = Quat::Identity();
            Quat endQ = Quat::FromToRotation(curDir, targetDir);
            Quat lerpQ = Quat::SLerp(begQ, endQ, Time::GetDeltaTime() * 3);
            Vec2 lerpDir = lerpQ * curDir;
            m_beamLock->GetTransform()->SetRight(lerpDir);
        }
    }

    if (m_animator->IsPlayingRoll() ||
        m_animator->IsPlayingAnimationAboutJump())
    {
        TrailEffect();
    }

    if (m_animator->IsPlayingJumpLand() || m_animator->IsPlayingRoll())
    {
        if (m_cloudDustCounter > 0)
        {
            m_cloudDustCounter -= Time::GetDeltaTime();
        }
        else
        {
            m_cloudDustCounter = 0.01f;
            if (m_animator->IsPlayingJumpLand())
                CloudDustEffect(GetDirection());
            else
                CloudDustEffect(-GetDirection());
        }
    }

    if (m_animator->IsPlayingIdle())
    {
        if (m_wait <= 0)
        {
            int pattern = rand() % 3;
            switch (pattern)
            {
                case 0:
                    GranadeShoot(3);
                    break;
                case 1:
                    DashForward();
                    break;
                case 2:
                    StartAimming(1);
                    break;
            }
        }
        else
        {
            m_wait -= Time::GetDeltaTime();
        }
    }
}

void Headhunter::Render()
{
    if (m_hp <= 0) return;

    if (Time::GetTimeScale() <= 0) return;
    if (m_animator->IsPlayingDashReady() && m_animator->GetPercent() > 0.3f && m_animator->GetPercent() < 0.9f)
    {
        auto line = GraphicDevice::GetLineDevice();
        Draw2DLineParameters param;
        param.worldScaleToWidth = true;
        param.width = 1.0f;
        param.color = Color(1, 0, 0, 0.8f);
        
        Vec2 beg = GetTransform()->GetPosition();
        Vec2 end;
        if (GetDirection() < 0) end = Vec2(m_leftWallX, GetTransform()->GetPosition().y);
        else end = Vec2(m_rightWallX, GetTransform()->GetPosition().y);

        line->Draw2DLine(beg + Vec2::down() * 5 , end + Vec2::down() * 5, &param);
    }
}

void Headhunter::OnDestroy()
{
    if (m_beamLock)
    {
        m_beamLock->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyBeamLock);
        m_beamLock->Cancel();
        m_beamLock = nullptr;
    }

    if (m_turretPattern)
    {
        m_turretPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndTurretPattern);
        m_turretPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_turretPattern = nullptr;
    }

    if (m_beamPattern)
    {
        m_beamPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndBeamPattern);
        m_beamPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_beamPattern = nullptr;
    }

    SAFE_DELETE(m_sprDashTrail);
}

void Headhunter::OnGrounded(const Vec2& point, const Vec2& normal)
{
    LandEffect();
}

void Headhunter::OnPlayEndHurtFlyLand()
{
    DisappearEffect();

    Hide();

    if (m_hp == (MAX_HP - 1))
    {
        auto o = new GameObject;
        m_turretPattern = o->AddComponent<HeadhunterTurretPattern>();
        m_turretPattern->OnEndPattern += Function<void>(this, &Headhunter::OnEndTurretPattern);
        m_turretPattern->OnDestoryCallback += Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
    }
    else
    {
        auto o = new GameObject;
        m_beamPattern = o->AddComponent<HeadhunterBeamPattern>();
        m_beamPattern->OnEndPattern += Function<void>(this, &Headhunter::OnEndBeamPattern);
        m_beamPattern->OnDestoryCallback += Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
    }
}

void Headhunter::OnPlayJump()
{
    JumpCloudEffect(GetBottom(), Vec2::up());

    float hd = GetDirection();

    // 뒤쪽으로 점프합니다.
    // 방향은 바꾸지 않습니다.
    m_body->SetVelocity(Vec2(-hd, 1.5f).normalized() * 550);
    DetachFromGround();

    m_animator->SetGround(false);
}

void Headhunter::OnWallGrab()
{
}

void Headhunter::OnWallJump()
{
    JumpCloudEffect(m_wallPoint, m_wallNormal);
    m_readyToPlayGatlingSound = true;

    int soundCase = rand() % 3;
    switch (soundCase)
    {
        case 0:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_walljump_01.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
        case 1:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_walljump_02.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
        case 2:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_walljump_03.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
    }
}

void Headhunter::OnPlayEndRifleReady()
{
    m_beamFireCounter = 0;
}

void Headhunter::OnPlayEndGranadeReady()
{
    m_animator->PlayGranadeShoot();
}

void Headhunter::OnPlayEndGranadeShoot()
{
    --m_granadeShootCount;
    if (m_granadeShootCount > 0)
        m_animator->PlayGranadeShoot();
    else
        m_animator->PlayGranadePutback();
}

void Headhunter::OnGranadeShootFrame()
{
    auto obj = new GameObject(GetTransform()->GetPosition());
    auto granade = obj->AddComponent<Granade>();

    float power = float(rand() % 300 + 700);
    float adjustUpAngle = float(rand() % 30 + 30);
    Vec2 dir = Vec2(GetDirection(), 0);
    dir = Quat::AxisAngle(Vec3::forawrd() * GetDirection(), adjustUpAngle * DEG2RAD) * dir;

    granade->SetVelocity(dir * power);
}

void Headhunter::OnPlayEndDashReady()
{
    DashWork(m_dashEnd);
}

void Headhunter::OnPlayEndRoll()
{
    float dodgeDirection;
    float minDistance = GetNearWallDistance(&dodgeDirection);
    float jumpDir = dodgeDirection;

    Backjump(jumpDir);
}

void Headhunter::OnDestroyBeamLock(Object* object)
{
    if (m_beamLock && m_beamLock == object)
    {
        m_beamLock->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyBeamLock);
        m_beamLock = nullptr;
    }
}

void Headhunter::OnEndBeamPattern()
{
    Show();
    WallJumpImmediately(-1.0f);
    m_body->UpdateInterpolation();

    if (m_beamPattern)
    {
        m_beamPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndBeamPattern);
        m_beamPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_beamPattern = nullptr;
    }
}

void Headhunter::OnEndTurretPattern()
{
    Show();
    DashDown(m_air);
    m_body->UpdateInterpolation();

    if (m_turretPattern)
    {
        m_turretPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndTurretPattern);
        m_turretPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_turretPattern = nullptr;
    }
}

void Headhunter::OnDie()
{
    auto o = new GameObject();
    auto dieEvent = o->AddComponent<HeadhunterDie>();
    dieEvent->SetPlayerController(PlayerController::GetInstance());
    dieEvent->SetHeadhunter(this);
    dieEvent->Play();
}

void Headhunter::OnDestroyReferenceObject(Object* object)
{
    if (!object) return;
    if (m_beamPattern && m_beamPattern == object)
    {
        m_beamPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndBeamPattern);
        m_beamPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_beamPattern = nullptr;
    }
    else if (m_turretPattern && m_turretPattern == object)
    {
        m_turretPattern->OnEndPattern -= Function<void>(this, &Headhunter::OnEndTurretPattern);
        m_turretPattern->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyReferenceObject);
        m_turretPattern = nullptr;
    }
}

void Headhunter::OnDamage()
{
    int soundCase = rand() % 3;
    switch (soundCase)
    {
        case 0:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_hurt_01.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
        case 1:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_hurt_02.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
        case 2:
            CSoundMgr::Get_Instance()->PlaySound(L"sound_voiceboss_huntress_hurt_03.wav", CSoundMgr::CHANNELID::BOSS_VOICE);
            break;
    }
}

SpriteRenderer* Headhunter::GetRenderer() const
{
    return m_renderer;
}

bool Headhunter::CanTakeDamage() const
{
    if (m_animator->IsPlayingHurt() ||
        m_animator->IsPlayingRoll() ||
        m_animator->IsPlayingJumpReady() ||
        m_animator->IsPlayingJumpLoop() ||
        m_animator->IsPlayingWallGrab() ||
        m_animator->IsPlayingDie()) return false;
    return true;
}

bool Headhunter::Damage(Vec2 velocity, Component* damageObject, bool canBlock)
{
    float hd = velocity.x < 0 ? -1.0f : +1.0f;

    m_body->SetVelocity(velocity);
    DetachFromGround();

    // 데미지가 오는 방향을 바라봅니다.
    SetDirection(-hd);

    m_animator->SetGround(false);
    m_animator->PlayHurt();

    BloodEffect(velocity.normalized());

    --m_hp;
    if (m_hp == 0)
    {
        m_animator->SetShouldDie();
        OnDie();
    }
    else
    {
        OnDamage();
    }

    return true;
}

void Headhunter::Roll(float hd)
{
    SetDirection(hd);
    m_body->SetVelocity(Vec2(hd, 0) * 500);
    m_animator->PlayRoll();
}

void Headhunter::Backjump(float hd)
{
    // hd 방향으로 점프합니다.
    // 이 때 hd의 반대 방향을 바라봅니다.

    SetDirection(-hd);
    m_animator->PlayJump();
}

void Headhunter::GranadeShoot(size_t count)
{
    if (count == 0) return;

    m_granadeShootCount = count;
    m_animator->PlayGranadeTakeout();
    LookPlayer();
}

void Headhunter::DashForward()
{
    LookPlayer();
    Dash(GetTransform()->GetPosition() + Vec2(GetDirection(), 0) * 1000);
}

void Headhunter::DashDown(const Vec2& start)
{
    GetTransform()->SetPosition(start);
    SetDirection(-GetDirection());

    RaycastResult res;
    if (Physics::Raycast(start, start + Vec2::down() * 1000, &res, LAYER_GROUND))
    {
        // 아래로 이동했을 때 정확한 y 위치를 구하기 위한 작업을 합니다.
        float boxhh = m_boxCollider->GetHeight() * 0.5f;
        float radius = m_circleCollider->GetRadius();
        float upLength = boxhh + radius;
        Dash(res.point + Vec2::up() * upLength, true);
    }
    else
    {
        // 거리를 1000으로 설정해서 무조건 바닥을 찾겠지만
        // 예상하지 못한 예외에는 그냥 무지성으로 아래쪽으로 꽂습니다.
        // 어차피 DashWork() 함수에서 한번 더 걸러줍니다.
        Dash(GetTransform()->GetPosition() + Vec2::down() * 1000, true);
    }
}

void Headhunter::StartAimming(size_t count)
{
    m_beamFireCount = count;
    m_animator->PlayAimming();
    LookPlayer();
}

void Headhunter::StopAimming()
{
    m_animator->PlayStopAimming();
}

void Headhunter::Dodge()
{
    float dodgeDirection;
    float minDistance = GetNearWallDistance(&dodgeDirection);
    float randomRoll = float(rand() % 100 + 1);

    float toPlayerDistance = fabsf(GetRelativeToPlayer().x);
    if (minDistance < 25 || // 벽이 너무 가깝거나
        minDistance > 150 || // 벽이 너무 멀거나
        toPlayerDistance < m_dodgeDistance - 25 || // 플레이어가 가까이 있을 때
        randomRoll > 80) // 랜덤하게 구르거나
    {
        if (toPlayerDistance < m_dodgeDistance - 25) // 플레이어가 가까이 있으면 벽의 반대 방향으로 구릅니다.
            Roll(-dodgeDirection);
        else if (minDistance > 150) // 벽이 너무 멀다면 벽쪽으로 구릅니다.
            Roll(dodgeDirection);
        else
            Roll(GetDirectionToPlayer()); // 플레이어 쪽으로 구릅니다.
    }
    else
    {
        Backjump(dodgeDirection);
    }
}

void Headhunter::WallJumpImmediately(float hd)
{
    m_animator->PlayWallGrab();
    m_body->SetVelocity(Vec2::zero());

    if (hd < 0)
    {
        GetTransform()->SetPosition(Vec2(m_leftWallX + GetWidth() * 0.5f, m_wallGrabY));
        SetDirection(-1.0f);
    }
    else
    {
        GetTransform()->SetPosition(Vec2(m_rightWallX - GetWidth() * 0.5f, m_wallGrabY));
        SetDirection(+1.0f);
    }

    RaycastToWall(hd);

    m_wallGrabCounter = 0;
    OnWallGrab();
}

void Headhunter::Hide()
{
    m_body->SetEnable(false);
    m_renderer->SetEnable(false);
    GetGameObject()->SetEnable(false);
}

void Headhunter::Show()
{
    m_body->SetEnable(true);
    m_renderer->SetEnable(true);
    GetGameObject()->SetEnable(true);
}

void Headhunter::Dash(const Vec2& end, bool fast)
{
    m_dashEnd = end;
    if(fast) m_animator->PlayFastDash();
    else m_animator->PlayDash();
}

void Headhunter::DashWork(const Vec2& end)
{
    Vec2 startPos = GetTransform()->GetPosition();
    Vec2 endPos;
    Vec2 relVec, dir;

    RaycastResult res;
    if (Physics::Raycast(startPos, end, &res, LAYER_GROUND))
    {
        endPos = res.point;
        relVec = endPos - startPos;
        dir = relVec.normalized();
        endPos -= dir * GetWidth();
    }
    else
    {
        endPos = end;
        relVec = endPos - startPos;
        dir = relVec.normalized();
    }

    float hd = 0;
    if (GetDirection() < 0) hd = -1.0f;
    else if (GetDirection() > 0) hd = +1.0f;
    else hd = 0;

    if (hd != 0) SetDirection(-hd);

    m_body->SetVelocity((endPos - startPos).normalized());
    m_body->SetPosition(endPos);

    constexpr int maxTrail = 100;
    for (int i = 0; i < maxTrail; ++i)
    {
        float percent = float(i) / float(maxTrail);
        Vec2 trailPos = Vec2::Lerp(startPos, endPos, percent);
        auto trail = TrailRenderer::Create(
            trailPos + Vec2::down() * 26, 
            m_sprDashTrail, 
            m_renderer->GetRenderOrder() - 1);
        trail->SetBeginColor(D3DCOLOR_ARGB(220, 255, 10, 50));
        trail->SetEndColor(D3DCOLOR_ARGB(0, 220, 30, 190));
        trail->SetDuration(percent * 0.3f);
        trail->SetAdditiveBlending(true);
    }

    auto Raycast = [&](const Vec2& _beg, const Vec2& _end) -> bool
    {
        std::vector<RaycastResult> results;
        Physics::RaycastAll(_beg, _end, &results, LAYER_CHARACTER);
        for (auto& resItem : results)
        {
            if (resItem.collider->GetBody()->GetGameObject()->GetTag() == TAG_PLAYER)
            {
                if (PlayerController::GetInstance()->IsRolling() || PlayerController::GetInstance()->IsHurt()) return true;
                PlayerController::GetInstance()->Damage(Vec2(-GetDirection(), 1).normalized() * 400, this);
                TimeController::GetInstance()->SetSubScale(0.05f, 0.2f);
                PlayerCamera::GetInstance()->Shake(GetTransform()->GetRight(), 0.2f, 600, 10);
                return true;
            }
        }
        return false;
    };

	Vec2 normal = Quat::AxisAngle(Vec3::forawrd(), 90 * DEG2RAD) * dir;
	if (Raycast(startPos, end)) return;
	if (Raycast(startPos, end)) return;
	if (Raycast(startPos + normal * 10, end + normal * 10)) return;
	if (Raycast(startPos - normal * 10, end - normal * 10)) return;
}

void Headhunter::CreateBeamLockWhenAimming()
{
    Vec2 point = GetTransform()->GetPosition();
    float angle = GetToPlayerAngle(point);
    auto o = new GameObject(point, angle);
    m_beamLock = o->AddComponent<BeamLock>();
    m_beamLock->SetLockTime(m_beamLockTime);
    m_beamLock->SetBeamDuration(m_beamDuration);
    m_beamLock->OnDestoryCallback += Function<void, Object*>(this, &Headhunter::OnDestroyBeamLock);
}

void Headhunter::CancelBeamLock()
{
    if (m_beamLock)
    {
        m_beamLock->OnDestoryCallback -= Function<void, Object*>(this, &Headhunter::OnDestroyBeamLock);
        m_beamLock->Cancel();
        m_beamLock = nullptr;
    }
}

float Headhunter::GetDirectionToPlayer() const
{
    Vec2 relVec = GetRelativeToPlayer();
    float hd = relVec.x < 0 ? -1.0f : +1.0f;
    return hd;
}

void Headhunter::LookPlayer()
{
    SetDirection(GetDirectionToPlayer());
}

Vec2 Headhunter::GetRelativeToPlayer() const
{
    Vec2 playerPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
    Vec2 curPos = GetTransform()->GetPosition();
    Vec2 relVec = playerPos - curPos;
    return relVec;
}

float Headhunter::GetToPlayerAngle(const Vec2& begin) const
{
    Vec2 relVec = GetRelativeToPlayer();
    return Vec2::Angle180(relVec.normalized());
}

float Headhunter::GetNearWallDistance(float* pHD) const
{
    float x = GetTransform()->GetPosition().x;
    float toLeftWall = fabsf(x - m_leftWallX);
    float toRightWall = fabsf(x - m_rightWallX);

    float minDistance;
    float toWallDirection;
    if (toLeftWall < toRightWall)
    {
        toWallDirection = -1.0f;
        minDistance = toLeftWall;
    }
    else
    {
        toWallDirection = +1.0f;
        minDistance = toRightWall;
    }
    *pHD = toWallDirection;
    return minDistance;
}

void Headhunter::DisappearEffect()
{
    constexpr int max_sparks = 20;
    for (int i = 0; i < max_sparks; ++i)
    {
        float percent = float(i) / float(max_sparks);
        float angle = percent * 360;
        float scale = float(rand() % 100) * 0.01f + 1.0f;
        wstring key;
        int key_state = rand() % 3;
        if (key_state == 0) key = L"../Animation/effect/spark0.txt";
        else if (key_state == 1) key = L"../Animation/effect/spark1.txt";
        else if (key_state == 2) key = L"../Animation/effect/spark2.txt";
        StaticEffect::Create(
            key,
            GetTransform()->GetPosition() + Vec2::Direction(angle * DEG2RAD) * float(rand() % 50),
            angle,
            Vec2::one() * scale,
            Vec2::zero(), Vec2::zero(),
            false,
            true)->SetSpeed(float(rand() % 100) * 0.01f + 0.5f);
    }
    constexpr int max_smoke = 20;
    for (int i = 0; i < max_smoke; ++i)
    {
        float percent = float(i) / float(max_smoke);
        float angle = percent * 360;
        float scale = float(rand() % 100) * 0.01f + 1.0f;
        wstring key;
        int key_state = rand() % 3;
        if (key_state == 0) key = L"../Animation/effect/gunsmoke0.txt";
        else if (key_state == 1) key = L"../Animation/effect/gunsmoke1.txt";
        else if (key_state == 2) key = L"../Animation/effect/gunsmoke2.txt";
        StaticEffect::Create(
            key,
            GetTransform()->GetPosition() + Vec2::Direction(angle * DEG2RAD) * float(rand() % 50 + 50),
            angle,
            Vec2::one() * scale,
            Vec2::zero(), Vec2::zero(),
            false,
            true)->SetSpeed(float(rand() % 100) * 0.01f + 0.5f);;
    }
    auto o = new GameObject(GetTransform()->GetPosition());
    o->GetTransform()->SetScale(Vec2(170, 170));
    auto damage = o->AddComponent<DamageZone>();
    damage->SetDamage(false);
    damage->SetAdjustVelocity(Vec2::up() * 200);
}

void Headhunter::BloodEffect(const Vec2& direction)
{
    for (int i = 0; i < 10 + rand() % 10; ++i)
    {
        Vec2 position = GetTransform()->GetPosition() + Vec2::up() * float(rand() % 20 - 10);
        position -= direction * 10;
        int bloodCase = rand() % 3;
        wstring key;
        switch (bloodCase)
        {
            case 0: key = L"../Animation/effect/blood_1.txt"; break;
            case 1: key = L"../Animation/effect/blood_2.txt"; break;
            case 2: key = L"../Animation/effect/blood_3.txt"; break;
        }
        float angle = Vec2::Angle180(-direction) + float(rand() % 90) - 45.0f;
        float scale = float(rand() % 100) * 0.01f + 0.5f;
        auto blood = StaticEffect::Create(
            key,
            position,
            angle,
            Vec2(-scale, scale),
            Vec2::Direction(angle * DEG2RAD) * 300,
            Vec2::down() * 500,
            false,
            true
        );
        blood->SetSpeed(float(rand() % 100) * 0.01f + 0.5f);
    }
}

bool Headhunter::RaycastToWall(float hd)
{
    Vec2 beg = GetTransform()->GetPosition();
    Vec2 end = beg + Vec2(hd, 0) * (m_circleCollider->GetRadius() + 1.0f);
    RaycastResult res;
    bool result = Physics::Raycast(beg, end, &res, LAYER_GROUND);
    m_wallPoint = res.point;
    m_wallNormal = res.normal;
    return result;
}

void Headhunter::TrailEffect()
{
    if (m_trailCounter > 0)
    {
        m_trailCounter -= Time::GetDeltaTime();
        return;
    }
    m_trailCounter = 0.01f;

    auto trail = TrailRenderer::Create(m_renderer);
    trail->SetBeginColor(D3DCOLOR_ARGB(220, 255, 10, 50));
    trail->SetEndColor(D3DCOLOR_ARGB(0, 220, 30, 190));
    trail->SetDuration(0.3f);
    trail->SetAdditiveBlending(true);
}

void Headhunter::CloudDustEffect(float hd)
{
    float scale = float(rand() % 100 + 1) * 0.01f + 0.5f;
    float adjust_distance = float(rand() % 10);
    auto effect = StaticEffect::Create(
        L"../Animation/effect/dustcloud.txt",
        GetBottom() + Vec2(hd, 0) * adjust_distance + Vec2::up() * 5,
        (hd < 0 ? 0.0f : 180.0f),
        Vec2::one() * scale);
    effect->SetVelocity(Vec2(-GetDirection(), 0.25f) * 100);
}

void Headhunter::JumpCloudEffect(const Vec2& point, const Vec2& normal)
{
    Vec2 p = point;
    p.y = GetTransform()->GetPosition().y;
    Quat q = Quat::FromToRotation(Vec2::up(), normal);
    auto effect = StaticEffect::Create(L"../Animation/effect/jumpcloud.txt", point, q.GetEulerAngle().z, Vec2::one());
    effect->SetSpeed(0.5f);
}

void Headhunter::LandEffect()
{
    StaticEffect::Create(L"../Animation/effect/landcloud.txt", GetBottom(), 0, Vec2::one() * 1.35f);
}

