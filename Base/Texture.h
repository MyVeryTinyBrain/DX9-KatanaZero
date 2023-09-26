#pragma once

class Texture
{
	friend class TextureManager;

	PRIVATE Texture(const LPDIRECT3DTEXTURE9& texture, const D3DXIMAGE_INFO& imageInfo);
	PRIVATE ~Texture();

	PRIVATE static Texture* Create(const wstring& filePath);
	PRIVATE static Texture* Create(const UINT& width, const UINT& height, const D3DXCOLOR& color);

	PUBLIC const D3DXIMAGE_INFO& GetImageInfo() const;
	PUBLIC LPDIRECT3DTEXTURE9 GetD3DXTexture() const;
	PUBLIC operator LPDIRECT3DTEXTURE9() const;
	PUBLIC const wstring& GetKey() const;

	PRIVATE D3DXIMAGE_INFO m_imageInfo;
	PRIVATE LPDIRECT3DTEXTURE9 m_texture;
	PRIVATE wstring m_key;
};

