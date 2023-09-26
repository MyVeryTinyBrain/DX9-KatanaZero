#include "base_stdafx.h"
#include "TextureManager.h"
#include "Texture.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(TextureManager)

IMPLEMENT_SINGLETON_CONSTRUCTOR(TextureManager)
{

}

IMPLEMENT_SINGLETON_DESTRUCTOR(TextureManager)
{
	Clear();
}

Texture* TextureManager::LoadFromFile(const wstring& filePath)
{
    // 삽입 시도를 합니다.
    // 반환하는 페어의 second는 이미 삽입하려는 키가 있는 경우 false, 아닌 경우 true를 반환합니다.
    std::pair<std::map<wstring, Texture*>::iterator, bool> result = INST.m_textures.insert({ filePath, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        // 키가 없는 경우에는 새로 생성하여 삽입 후 반환합니다.
        Texture* texture = Texture::Create(filePath);
        texture->m_key = filePath;
        result.first->second = texture;
        return texture;
    }
    else
    {
        // 키가 있는 경우에는 이미 존재하는 텍스쳐를 반환합니다.
        return result.first->second;
    }
}

Texture* TextureManager::Create(const wstring& key, const UINT& width, const UINT& height, const D3DXCOLOR& color)
{
    std::pair<std::map<wstring, Texture*>::iterator, bool> result = INST.m_textures.insert({ key, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        Texture* texture = Texture::Create(width, height, color);
        texture->m_key = key;
        result.first->second = texture;
        return texture;
    }
    else
    {
        return result.first->second;
    }
}

Texture* TextureManager::GetTexture(const wstring& key)
{
    auto it = INST.m_textures.find(key);
    if (it == INST.m_textures.end()) return nullptr;
    return it->second;
}

void TextureManager::Clear()
{
    for (auto& pair : INST.m_textures)
        SAFE_DELETE(pair.second);
    INST.m_textures.clear();
}
