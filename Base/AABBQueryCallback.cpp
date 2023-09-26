#include "base_stdafx.h"
#include "AABBQueryCallback.h"
#include "Box2dHeader.h"
#include "Collider.h"

AABBQueryCallback::AABBQueryCallback()
{
	m_useLayerMask = false;
	m_layerMask = 0;
}

AABBQueryCallback::AABBQueryCallback(layer_t layerMask)
{
	m_useLayerMask = true;
	m_layerMask = layerMask;
}

bool AABBQueryCallback::ReportFixture(b2Fixture* fixture)
{
	// ���̾� ����ũ�� ����Ѵٸ� �ش� ���̾� ��Ʈ�� ���� ���� ������ �� �ݶ��̴��� ���� ó���� ���� �ʽ��ϴ�.
	if (m_useLayerMask)
	{
		const b2Filter& filter = fixture->GetFilterData();
		bool valid = filter.categoryBits & m_layerMask;
		if (!valid) return true;
	}

	Collider* collider = (Collider*)fixture->GetUserData().pointer;
	if (!collider->GetActive())
	{
		return true;
	}

	m_colliders.push_back(collider);
	return true;
}

void AABBQueryCallback::MoveColliders(std::vector<Collider*>* pColliders)
{
	*pColliders = std::move(m_colliders);
}
