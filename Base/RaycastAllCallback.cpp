#include "base_stdafx.h"
#include "RaycastAllCallback.h"
#include "Box2dHeader.h"
#include "Collider.h"

RaycastAllCallback::RaycastAllCallback()
{
	m_useLayerMask = false;
	m_layerMask = 0;
}

RaycastAllCallback::RaycastAllCallback(layer_t layerMask)
{
	m_useLayerMask = true;
	m_layerMask = layerMask;
}

float RaycastAllCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
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

	RaycastResult result;
	result.collider = collider;
	result.point = B2VEC2_TO_VEC2(point);
	result.normal = B2VEC2_TO_VEC2(normal);
	m_results.push_back(result);
	return 1.0f;
}

const std::vector<RaycastResult>& RaycastAllCallback::GetResults() const
{
	return m_results;
}

void RaycastAllCallback::MoveResults(std::vector<RaycastResult>* pResults)
{
	*pResults = std::move(m_results);
}
