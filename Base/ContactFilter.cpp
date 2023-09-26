#include "base_stdafx.h"
#include "ContactFilter.h"
#include "Collider.h"
#include "GameObject.h"
#include "Box2dHeader.h"

bool ContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    bool collide = b2ContactFilter::ShouldCollide(fixtureA, fixtureB);
    if (!collide) return false;

    // �� �ݶ��̴��� ���ؼ�
    // ������ ���ӿ�����Ʈ�� Ȱ��ȭ �Ǿ�������
    // �ݶ��̴� ���� Ȱ��ȭ ������ ��
    // �浹�� �� �ֽ��ϴ�.

    Collider* colliderA = (Collider*)fixtureA->GetUserData().pointer;
    Collider* colliderB = (Collider*)fixtureB->GetUserData().pointer;
    return colliderA->GetActive() && colliderB->GetActive();
}
