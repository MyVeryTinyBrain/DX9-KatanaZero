#include "base_stdafx.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Texture.h"

Sprite::Sprite(const Sprite& sprite)
{
	m_texture = sprite.m_texture;
	m_type = sprite.m_type;
	U = sprite.U;
}

Sprite::Sprite(wstring textureKey)
{
	Texture* texture = TextureManager::GetTexture(textureKey);
	if (texture)
	{
		m_texture = texture;
		m_type = SpriteType::Single;
		ZeroMemory(&U, sizeof(U));
	}
	else
	{
		wstring msg = L"sprite missing: " + textureKey;
		wcout << msg << endl;
	}
}

Sprite::Sprite(Texture* texture)
{
	m_texture = texture;
	m_type = SpriteType::Single;
	ZeroMemory(&U, sizeof(U));
}

Sprite::Sprite(Texture* texture, const RECT& slicedRect, bool dontChangeCenter)
{
	m_texture = texture;
	m_type = SpriteType::SlicedRect;
	ZeroMemory(&U, sizeof(U));
	U.USlicedRect.slicedRect = slicedRect;
	U.USlicedRect.dontChangeCenter = dontChangeCenter;
}

Sprite::Sprite(Texture* texture, const size_t& countX, const size_t& countY, const size_t& indexX, const size_t& indexY)
{
	m_texture = texture;
	m_type = SpriteType::SlicedXYIndex;
	ZeroMemory(&U, sizeof(U));
	U.USlicedXYIndex.countX = countX;
	U.USlicedXYIndex.countY = countY;
	U.USlicedXYIndex.indexX = indexX;
	U.USlicedXYIndex.indexY = indexY;
}

Sprite::Sprite(Texture* texture, const size_t& countX, const size_t& countY, const size_t& index)
{
	m_texture = texture;
	m_type = SpriteType::SlicedIndex;
	ZeroMemory(&U, sizeof(U));
	U.USlidexIndex.countX = countX;
	U.USlidexIndex.countY = countY;
	U.USlidexIndex.index = index;
}

Sprite::~Sprite()
{
}

Texture* Sprite::GetTexture() const
{
	return m_texture;
}

const SpriteType& Sprite::GetSpriteType() const
{
	return m_type;
}

Vec2 Sprite::GetSize() const
{
	switch (m_type)
	{
		case SpriteType::Single:
			return { float(m_texture->GetImageInfo().Width),float(m_texture->GetImageInfo().Height) };
		case SpriteType::SlicedRect:
			return { float(U.USlicedRect.slicedRect.right - U.USlicedRect.slicedRect.left),float(U.USlicedRect.slicedRect.bottom - U.USlicedRect.slicedRect.top) };
		case SpriteType::SlicedXYIndex:
			return { float(m_texture->GetImageInfo().Width / U.USlicedXYIndex.countX), float(m_texture->GetImageInfo().Height / U.USlicedXYIndex.countY) };
		case SpriteType::SlicedIndex:
			return { float(m_texture->GetImageInfo().Width / U.USlicedXYIndex.countX), float(m_texture->GetImageInfo().Height / U.USlicedXYIndex.countY) };
		default: return {};
	}
}

const RECT& Sprite::GetSlicedRect() const
{
	return U.USlicedRect.slicedRect;
}

bool Sprite::IsDontChangedCenter() const
{
	return U.USlicedRect.dontChangeCenter;
}

const size_t& Sprite::GetIndexX() const
{
	return U.USlicedXYIndex.indexX;
}

const size_t& Sprite::GetIndexY() const
{
	return U.USlicedXYIndex.indexY;
}

const size_t& Sprite::GetIndex() const
{
	return U.USlidexIndex.index;
}

const size_t& Sprite::GetCountX() const
{
	switch (m_type)
	{
		case SpriteType::SlicedXYIndex:
			return U.USlicedXYIndex.countX;
		case SpriteType::SlicedIndex:
			return U.USlidexIndex.countX;
	}
	return U.USlicedXYIndex.countX;
}

const size_t& Sprite::GetCountY() const
{
	switch (m_type)
	{
		case SpriteType::SlicedXYIndex:
			return U.USlicedXYIndex.countY;
		case SpriteType::SlicedIndex:
			return U.USlidexIndex.countY;
	}
	return U.USlicedXYIndex.countY;
}

void Sprite::SetSlicedRect(const RECT& rect)
{
	U.USlicedRect.slicedRect = rect;
}

void Sprite::SetDontChangeCenter(bool enable)
{
	U.USlicedRect.dontChangeCenter = enable;
}

void Sprite::SetIndexX(const size_t& x)
{
	U.USlicedXYIndex.indexX = x;
}

void Sprite::SetIndexY(const size_t& y)
{
	U.USlicedXYIndex.indexY = y;
}

void Sprite::SetIndex(const size_t& i)
{
	U.USlidexIndex.index = i;
}
