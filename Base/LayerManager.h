#pragma once

class LayerManager
{
	DECLARE_SINGLETON(LayerManager)

	PUBLIC static void SetCollision(unsigned int IndexA, unsigned int IndexB, bool collision);
	PUBLIC static bool GetCollision(unsigned int IndexA, unsigned int IndexB);

	PUBLIC void Reset();
	PUBLIC static layer_t GetLayer(unsigned int Index);
	PUBLIC static unsigned int GetLayerIndex(layer_t layer);
	PUBLIC static layer_t GetCollisionLayer(unsigned int Index);

	PRIVATE bool m_collisionMatrix[16][16];

	PUBLIC static Delegate<void> OnLayerChanged;
};

