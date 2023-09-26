#include "base_stdafx.h"
#include "SpriteDevice.h"
#include "Texture.h"
#include "Sprite.h"

#include "GraphicDevice.h"
#include "Input.h"

SpriteDevice* SpriteDevice::Create(LPDIRECT3DDEVICE9 device)
{
	LPD3DXSPRITE sprite = nullptr;
	if (FAILED(D3DXCreateSprite(device, &sprite)))
		return nullptr;
	return new SpriteDevice(sprite);
}

SpriteDevice::SpriteDevice(LPD3DXSPRITE sprite)
{
	m_sprite = sprite;
}

SpriteDevice::~SpriteDevice()
{
	SAFE_RELEASE(m_sprite);
}

HRESULT SpriteDevice::BeginRender()
{
	return m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

HRESULT SpriteDevice::EndRender()
{
	return m_sprite->End();
}

void SpriteDevice::DrawSingleTextrue(
	Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color)
{
	m_sprite->SetTransform(&transform);
	// 텍스쳐 한 장의 중심을 구합니다.
	// 중심 좌표는 로컬 좌표입니다.
	D3DXVECTOR3 center(texture->GetImageInfo().Width * 0.5f, texture->GetImageInfo().Height * 0.5f, 0);
	HRESULT res = m_sprite->Draw
	(
		(*texture),
		nullptr,
		&center,
		nullptr,
		color
	);
}

void SpriteDevice::DrawSheetTextrue(
	Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
	const size_t& countX, const size_t& countY, const size_t& indexX, const size_t& indexY)
{
	m_sprite->SetTransform(&transform);
	// 시트의 분할된 너비와 높이를 구합니다.
	float oneWidth = texture->GetImageInfo().Width / (float)countX;
	float oneHeight = texture->GetImageInfo().Height / (float)countY;
	// 시트의 그릴 영역을 구합니다.
	RECT srcRect{ LONG(oneWidth * indexX), LONG(oneHeight * indexY), LONG(oneWidth * (indexX + 1)), LONG(oneHeight * (indexY + 1)) };
	// 그릴 영역의 중심을 구합니다.
	// 중심 좌표는 로컬 좌표입니다.
	D3DXVECTOR3 center(oneWidth * 0.5f, oneHeight * 0.5f, 0);
	m_sprite->Draw
	(
		(*texture),
		&srcRect,
		&center,
		nullptr,
		color
	);
}

void SpriteDevice::DrawSheetTextrue(
	Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
	const size_t& countX, const size_t& countY, const size_t& index)
{
	// 1차원 인덱스를 2차원 인덱스로 변환합니다.
	size_t indexX = index % countX;
	size_t indexY = index / countX;
	DrawSheetTextrue(texture, transform, color, countX, countY, indexX, indexY);
}

void SpriteDevice::DrawRectTexture(
	Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color,
	const RECT& srcRect)
{
	m_sprite->SetTransform(&transform);
	float oneWidth = float(srcRect.right - srcRect.left);
	float oneHeight = float(srcRect.bottom - srcRect.top);
	// 그릴 영역의 중심을 구합니다.
	// 중심 좌표는 로컬 좌표입니다.
	D3DXVECTOR3 center(oneWidth * 0.5f, oneHeight * 0.5f, 0);
	m_sprite->Draw
	(
		(*texture),
		&srcRect,
		&center,
		nullptr,
		color
	);
}

void SpriteDevice::DrawRectTextureDontChangeCenter(Texture* texture, const D3DXMATRIX& transform, const D3DXCOLOR& color, const RECT& srcRect)
{
	D3DXVECTOR3 center(texture->GetImageInfo().Width * 0.5f, texture->GetImageInfo().Height * 0.5f, 0);
	m_sprite->Draw
	(
		(*texture),
		&srcRect,
		&center,
		nullptr,
		color
	);
}

void SpriteDevice::DrawSprite(Sprite* sprite, const D3DXMATRIX& transform, const D3DXCOLOR& color)
{
	const SpriteType& type = sprite->GetSpriteType();
	switch (type)
	{
		case SpriteType::Single:
			DrawSingleTextrue(sprite->GetTexture(), transform, color);
			break;
		case SpriteType::SlicedXYIndex:
			DrawSheetTextrue(sprite->GetTexture(), transform, color, sprite->GetCountX(), sprite->GetCountY(), sprite->GetIndexX(), sprite->GetIndexY());
			break;
		case SpriteType::SlicedIndex:
			DrawSheetTextrue(sprite->GetTexture(), transform, color, sprite->GetCountX(), sprite->GetCountY(), sprite->GetIndex());
			break;
		case SpriteType::SlicedRect:
			if (sprite->IsDontChangedCenter()) DrawRectTextureDontChangeCenter(sprite->GetTexture(), transform, color, sprite->GetSlicedRect());
			else DrawRectTexture(sprite->GetTexture(), transform, color, sprite->GetSlicedRect());
			break;
	}
}


LPDIRECT3DDEVICE9 SpriteDevice::GetDevice() const
{
	LPDIRECT3DDEVICE9 device;
	m_sprite->GetDevice(&device);
	return device;
}

LPD3DXSPRITE SpriteDevice::GetSprite() const
{
	return m_sprite;
}
