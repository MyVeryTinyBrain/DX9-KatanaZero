#pragma once

class Texture;
class Sprite;

class SpriteDevice
{
	PUBLIC static SpriteDevice* Create(LPDIRECT3DDEVICE9 device);
	PRIVATE SpriteDevice(LPD3DXSPRITE sprite);
	PUBLIC ~SpriteDevice();

	// ��������Ʈ ��ġ�� ������ ������ �� ȣ���մϴ�.
	PUBLIC HRESULT BeginRender();

	// ��������Ʈ ��ġ�� ������ ���� �� ȣ���մϴ�.
	PUBLIC HRESULT EndRender();

	// �ؽ��ĸ� ��� �׸��ϴ�.
	PUBLIC void DrawSingleTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color
	);

	// ��Ʈ �ؽ����� ������ x, y �ε����� �κ��� �׸��ϴ�.
	PUBLIC void DrawSheetTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const size_t& countX, const size_t& countY, const size_t& indexX, const size_t& indexY
	);

	// ��Ʈ �ؽ����� ������ �ε����� �κ��� �׸��ϴ�.
	PUBLIC void DrawSheetTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const size_t& countX, const size_t& countY, const size_t& index
	);

	// �ؽ����� ������ �κ��� �׸��ϴ�.
	PUBLIC void DrawRectTexture(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const RECT& srcRect
	);

	// �ؽ����� ������ �κ��� �׸��ϴ�.
	PUBLIC void DrawRectTextureDontChangeCenter(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const RECT& srcRect
	);

	// ��������Ʈ�� ������ ���� �׸��ϴ�.
	PUBLIC void DrawSprite(
		Sprite* sprite, const D3DXMATRIX& transform, const D3DXCOLOR& color
	);

	PUBLIC LPDIRECT3DDEVICE9 GetDevice() const;
	PUBLIC LPD3DXSPRITE GetSprite() const;

	PRIVATE LPD3DXSPRITE m_sprite;
};

