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
	// 레이어 마스크를 사용한다면 해당 레이어 비트가 켜져 있지 않을때 이 콜라이더에 대해 처리를 하지 않습니다.
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
	가장 가까운 교차점만 찾으려면 :
	- 콜백에서 분수 값 반환
	- 가장 최근 교차점을 결과로 사용
	광선을 따라 모든 교차점을 찾으려면 :
	- 콜백에서 1 반환
	- 목록에 교차점 저장
	광선이 무엇이든 맞는지 간단히 찾으려면 :
	- 콜백을 받으면 무언가가 맞았습니다 (그러나 가장 가깝지 않을 수 있음)
	- 효율성을 위해 콜백에서 0을 반환합니다.
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
