#pragma once

class PlayerAnimator;
class PlatformerBody; 
class ThrowObject;

class PlayerController : public Component
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(FixedUpdate);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(BeforeRender);
	COMPONENT_OVERRIDE(OnDestroy);

	PRIVATE void OnGround(const Vec2& normal, const Vec2& point);
	PRIVATE void OnJump(const Vec2& normal, const Vec2& point);
	PRIVATE void OnFlip(const Vec2& normal, const Vec2& point);
	PRIVATE void OnEndPlayHurtflyGround();

	PUBLIC static PlayerController* GetInstance();

	PUBLIC PlatformerBody* GetPlatformerBody() const;

	PUBLIC void SetDirection(float hd);
	PUBLIC float GetDirection() const;
	PUBLIC void Knockback(Vec2 velocity, Component* damageObject);
	PUBLIC void HardKnockback(Vec2 velocity, Component* damageObject);
	PUBLIC void Damage(Vec2 velocity, Component* damageObject);
	PUBLIC bool IsRolling() const;
	PUBLIC bool IsHurt() const;

	PUBLIC bool CanPutInThrowObject() const;
	PUBLIC void PutInThrowObject(ThrowObject* object);
	PUBLIC void ThrowHasObject();

	PUBLIC SpriteRenderer* GetRenderer() const;

	PUBLIC void SetSlowControll(bool enable);
	PUBLIC void SetSlowModeColorChange(bool enable);

	PRIVATE void MakeTrailEffect();

	PRIVATE void HorizontalMove();
	PRIVATE void Jump();
	PRIVATE void LongJump();
	PRIVATE void Attack();
	PRIVATE void RollAndThroughPlatform();
	PRIVATE void Crouch();
	PRIVATE void WallGrab();
	PRIVATE void Flip();
	PRIVATE void Hurt(Vec2 velocity, Component* damageObject, bool canWake);
	PRIVATE void HurtCover();
	PRIVATE void DoorOpen();

	PRIVATE void Slow();
	PRIVATE void LimitTime();
	
	PRIVATE void Restart();

	PRIVATE bool RaycastToWall(bool left, float length, RaycastResult* pResult);

	PRIVATE SpriteRenderer* m_renderer;
	PRIVATE PlayerAnimator* m_animator;
	PRIVATE PlatformerBody* m_body;

	PRIVATE Vec2 m_reservedVel;
	PRIVATE Vec2 m_accelDir;
	PRIVATE bool m_hasJump;
	PRIVATE bool m_pressingJump;
	PRIVATE Vec2 m_rollDirection;
	PRIVATE bool m_crouching;
	PRIVATE float m_jumpGravityScale;
	PRIVATE bool m_wallGrabbing;
	PRIVATE float m_wallGrabGragivtScale;
	PRIVATE Vec2 m_reservedFlipVel;
	PRIVATE float m_knockbackCounter;
	PRIVATE Vec2 m_grabWallNormal;
	PRIVATE Vec2 m_grabWallPoint;
	PRIVATE float m_dustCreationCounter;
	PRIVATE Vec2 m_prevVel;
	PRIVATE float m_slowTimeTrailCounter;

	PRIVATE float m_maxSlowTime;
	PRIVATE float m_slowCounter;
	PRIVATE bool m_slowRecovery;
	PRIVATE float m_slowRecoveryTime;
	PRIVATE float m_slowRecoveryCounter;
	PRIVATE bool m_slow;
	PRIVATE bool m_useSlowControll;
	PRIVATE bool m_slowModeColorChange;

	PRIVATE float m_maxLimitTime;
	PRIVATE float m_limitCounter;

	PRIVATE bool m_wasPutInThrowObjectPrevFrame;
	PRIVATE ThrowObject* m_throwObject;

	PRIVATE bool m_isDead;

	PRIVATE float m_trailCounter;
	PUBLIC bool godMode = false;

	PRIVATE static PlayerController* g_instance;
};

