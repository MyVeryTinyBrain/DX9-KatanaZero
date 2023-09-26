#pragma once

#include "Boss.h"

class HeadHunterAnimator;
class BeamLock;
class HeadhunterTurretPattern;
class HeadhunterBeamPattern;

class Headhunter : public Boss
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Start);
	COMPONENT_OVERRIDE(BeforeFixedStep);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);
	PRIVATE virtual void OnGrounded(const Vec2& point, const Vec2& normal);
	PRIVATE void OnPlayEndHurtFlyLand();
	PRIVATE void OnPlayJump();
	PRIVATE void OnWallGrab();
	PRIVATE void OnWallJump();
	PRIVATE void OnPlayEndRifleReady();
	PRIVATE void OnPlayEndGranadeReady();
	PRIVATE void OnPlayEndGranadeShoot();
	PRIVATE void OnGranadeShootFrame();
	PRIVATE void OnPlayEndDashReady();
	PRIVATE void OnPlayEndRoll();
	PRIVATE void OnDestroyBeamLock(Object* object);
	PRIVATE void OnEndBeamPattern();
	PRIVATE void OnEndTurretPattern();
	PRIVATE void OnDie();
	PRIVATE void OnDestroyReferenceObject(Object* object);
	PRIVATE void OnDamage();

	PUBLIC SpriteRenderer* GetRenderer() const;

	PRIVATE virtual bool CanTakeDamage() const override;
	PRIVATE virtual bool Damage(Vec2 velocity, Component* damageObject, bool canBlock = false) override;

	PRIVATE void Roll(float hd);
	PRIVATE void Backjump(float hd);
	PRIVATE void GranadeShoot(size_t count);
	PRIVATE void DashForward();
	PRIVATE void DashDown(const Vec2& start);
	PRIVATE void StartAimming(size_t count);
	PRIVATE void StopAimming();
	PRIVATE void Dodge();
	PRIVATE void WallJumpImmediately(float hd);
	PRIVATE void Hide();
	PRIVATE void Show();

	PRIVATE void Dash(const Vec2& end, bool fast = false);
	PRIVATE void DashWork(const Vec2& end);
	PRIVATE void CreateBeamLockWhenAimming();
	PRIVATE void CancelBeamLock();

	PRIVATE float GetDirectionToPlayer() const;
	PRIVATE void LookPlayer();
	PRIVATE Vec2 GetRelativeToPlayer() const;
	PRIVATE float GetToPlayerAngle(const Vec2& begin) const;
	PRIVATE float GetNearWallDistance(float* pHD) const;

	PRIVATE void DisappearEffect();
	PRIVATE void BloodEffect(const Vec2& direction);
	PRIVATE bool RaycastToWall(float hd);
	PRIVATE void TrailEffect();
	PRIVATE void CloudDustEffect(float hd);
	PRIVATE void JumpCloudEffect(const Vec2& point, const Vec2& normal);
	PRIVATE void LandEffect();

	PRIVATE HeadHunterAnimator* m_animator;

	PRIVATE float m_wait;

	PRIVATE float m_wallGrabDelay;
	PRIVATE float m_wallGrabCounter;
	PRIVATE Vec2 m_wallPoint;
	PRIVATE Vec2 m_wallNormal;
	PRIVATE int m_granadeShootCount;
	PRIVATE Vec2 m_dashEnd;
	PRIVATE Sprite* m_sprDashTrail;
	PRIVATE float m_trailCounter;
	PRIVATE int m_bulletStep;
	PRIVATE float m_beamFireCounter;
	PRIVATE float m_beamLockTime;
	PRIVATE float m_beamDuration;
	PRIVATE BeamLock* m_beamLock;
	PRIVATE int m_beamFireCount;
	PRIVATE float m_dodgeDistance;
	PRIVATE float m_cloudDustCounter;

	PRIVATE float m_leftWallX;
	PRIVATE float m_rightWallX;
	PRIVATE float m_floorY;
	PRIVATE Vec2 m_air;
	PRIVATE float m_wallGrabY;

	PRIVATE bool m_readyToPlayGatlingSound;

	PRIVATE int m_hp;

	PRIVATE HeadhunterTurretPattern* m_turretPattern;
	PRIVATE HeadhunterBeamPattern* m_beamPattern;
};

