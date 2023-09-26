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
    // ���� �õ��� �մϴ�.
    // ��ȯ�ϴ� ����� second�� �̹� �����Ϸ��� Ű�� �ִ� ��� false, �ƴ� ��� true�� ��ȯ�մϴ�.
    std::pair<std::map<wstring, Texture*>::iterator, bool> result = INST.m_textures.insert({ filePath, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        // Ű�� ���� ��쿡�� ���� �����Ͽ� ���� �� ��ȯ�մϴ�.
        Texture* texture = Texture::Create(filePath);
        texture->m_key = filePath;
        result.first->second = texture;
        return texture;
    }
    else
    {
        // Ű�� �ִ� ��쿡�� �̹� �����ϴ� �ؽ��ĸ� ��ȯ�մϴ�.
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
