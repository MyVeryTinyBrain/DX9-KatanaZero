#pragma once

class Texture;

class TextureManager
{
	DECLARE_SINGLETON(TextureManager)

	PUBLIC static Texture* LoadFromFile(const wstring& filePath);
	PUBLIC static Texture* Create(const wstring& key, const UINT& width, const UINT& height, const D3DXCOLOR& color);
	PUBLIC static Texture* GetTexture(const wstring& key);
	PUBLIC static void Clear();

	PRIVATE std::map<wstring, Texture*> m_textures;
};