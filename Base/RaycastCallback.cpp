#include "base_stdafx.h"
#include "RaycastCallback.h"
#include "Box2dHeader.h"
#include "Collider.h"

RaycastCallback::RaycastCallback()
{
	m_collision = false;
	m_result = {};
	m_fraction = 0;
	m_useLayerMask = false;
	m_layerMask = 0;
}

RaycastCallback::RaycastCallback(layer_t layerMask)
{
	m_collision = false;
	m_result = {};
	m_fraction = 0;
	m_useLayerMask = true;
	m_layerMask = layerMask;
}

float RaycastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	// ���̾� ����ũ�� ����Ѵٸ� �ش� ���̾� ��Ʈ�� ���� ���� ������ �� �ݶ��̴��� ���� ó���� ���� �ʽ��ϴ�.
	if (m_useLayerMask)
	{
		const b2Filter& filter = fixture->GetFilterData();
		bool valid = filter.categoryBits & m_layerMask;
		if (!valid) return 1;
	}

	Collider* collider = (Collider*)fixture->GetUserData().pointer;
	if (!collider->GetActive())
	{
		return 1;
	}

	m_collision = true;
	m_result.collider = collider;
	m_result.point = B2VEC2_TO_VEC2(point);
	m_result.normal = B2VEC2_TO_VEC2(normal);
	m_fraction = fraction;

	/*
	���� ����� �������� ã������ :
	- �ݹ鿡�� �м� �� ��ȯ
	- ���� �ֱ� �������� ����� ���
	������ ���� ��� �������� ã������ :
	- �ݹ鿡�� 1 ��ȯ
	- ��Ͽ� ������ ����
	������ �����̵� �´��� ������ ã������ :
	- �ݹ��� ������ ���𰡰� �¾ҽ��ϴ� (�׷��� ���� ������ ���� �� ����)
	- ȿ������ ���� �ݹ鿡�� 0�� ��ȯ�մϴ�.
	*/

	return fraction;
}

const bool& RaycastCallback::HasCollision() const
{
	return m_collision;
}

const RaycastResult& RaycastCallback::GetResult() const
{
	return m_result;
}

const float& RaycastCallback::GetFraction() const
{
	return m_fraction;
}
