#include "base_stdafx.h"
#include "ContactFilter.h"
#include "Collider.h"
#include "GameObject.h"
#include "Box2dHeader.h"

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    bool collide = b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
    if (!collide) return false;

    // 두 콜라이더에 대해서
    // 부착된 게임오브젝트가 활성화 되어있으며
    // 콜라이더 역시 활성화 상태일 때
    // 충돌할 수 있습니다.

    Collider* colliderA = (Collider*)fixtureA->GetUserData().pointer;
    Collider* colliderB = (Collider*)fixtureB->GetUserData().pointer;
    return colliderA->GetActive() && colliderB->GetActive();
}
