#include "base_stdafx.h"
#include "LayerManager.h"

#define INST (g_instance)

Delegate<void> LayerManager::OnLayerChanged;

IMPLEMENT_SINGLETON(LayerManager)

IMPLEMENT_SINGLETON_CONSTRUCTOR(LayerManager)
{
	Reset();
}

IMPLEMENT_SINGLETON_DESTRUCTOR(LayerManager)
{
	OnLayerChanged.clear();
}

void LayerManager::SetCollision(unsigned int IndexA, unsigned int IndexB, bool collision)
{
	// 충돌하려는 설정이 이미 설정된 값과 같은 경우 종료합니다.
	if (INST.m_collisionMatrix[IndexA][IndexB] == collision) return;
	if (INST.m_collisionMatrix[IndexB][IndexA] == collision) return;

	INST.m_collisionMatrix[IndexA][IndexB] = collision;
	INST.m_collisionMatrix[IndexB][IndexA] = collision;

	// 레이어 충돌 변경 이벤트를 호출합니다.
	OnLayerChanged();
}

bool LayerManager::GetCollision(unsigned int IndexA, unsigned int IndexB)
{
	return INST.m_collisionMatrix[IndexA][IndexB];
}

void LayerManager::Reset()
{
	ZERO_MEM(m_collisionMatrix);
	for (int i = 0; i < 16; ++i)
	{
		SetCollision(0, i, true);
	}
}

layer_t LayerManager::GetLayer(unsigned int Index)
{
	return 1 << Index;
}

unsigned int LayerManager::GetLayerIndex(layer_t layer)
{
	for (int i = 0; i < 16; ++i)
	{
		layer_t And = (1 << i) & layer;
		if (And > 0) return i;
	}
	return 0;
}

layer_t LayerManager::GetCollisionLayer(unsigned int Index)
{
	layer_t collision = 0x00;
	for (int i = 0; i < 16; ++i)
	{
		if (INST.m_collisionMatrix[Index][i] == true)
		{
			collision |= (1 << i);
		}
	}
	return collision;
}
