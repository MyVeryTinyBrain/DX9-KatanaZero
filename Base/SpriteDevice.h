#pragma once

class Texture;
class Sprite;

class SpriteDevice
{
	PUBLIC static SpriteDevice* Create(LPDIRECT3DDEVICE9 device);
	PRIVATE SpriteDevice(LPD3DXSPRITE sprite);
	PUBLIC ~SpriteDevice();

	// 스프라이트 장치로 렌더를 시작할 때 호출합니다.
	PUBLIC HRESULT BeginRender();

	// 스프라이트 장치로 렌더를 끝낼 때 호출합니다.
	PUBLIC HRESULT EndRender();

	// 텍스쳐를 모두 그립니다.
	PUBLIC void DrawSingleTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color
	);

	// 시트 텍스쳐의 지정된 x, y 인덱스의 부분을 그립니다.
	PUBLIC void DrawSheetTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const size_t& countX, const size_t& countY, const size_t& indexX, const size_t& indexY
	);

	// 시트 텍스쳐의 지정된 인덱스의 부분을 그립니다.
	PUBLIC void DrawSheetTextrue(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const size_t& countX, const size_t& countY, const size_t& index
	);

	// 텍스쳐의 지정된 부분을 그립니다.
	PUBLIC void DrawRectTexture(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const RECT& srcRect
	);

	// 텍스쳐의 지정된 부분을 그립니다.
	PUBLIC void DrawRectTextureDontChangeCenter(
		Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
		const RECT& srcRect
	);

	// 스프라이트의 설정에 따라 그립니다.
	PUBLIC void DrawSprite(
		Sprite* sprite, const D3DXMATRIX& transform, const D3DXCOLOR& color
	);

	PUBLIC LPDIRECT3DDEVICE9 GetDevice() const;
	PUBLIC LPD3DXSPRITE GetSprite() const;

	PRIVATE LPD3DXSPRITE m_sprite;
};

