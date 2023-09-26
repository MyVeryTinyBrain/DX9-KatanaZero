#include "stdafx.h"
#include "ResourceLoader.h"
#include "AnimationManager.h"

namespace fs = std::filesystem;
using std::stack;
using std::vector;

bool ResourceLoader::g_loadComplete = false;

void ResourceLoader::GetFilePathes(
	const wstring& beginLocalDirectory,
	const wstring& extension,
	std::vector<wstring>* pPathes)
{
	fs::path begin = beginLocalDirectory;
	for (const fs::directory_entry& entry : fs::recursive_directory_iterator(begin))
	{
		const fs::path& path = entry.path();
		bool is_directory = fs::is_directory(path);
		if (is_directory) continue;
		bool is_unused = path.wstring().find(L"unused") != wstring::npos;
		if (is_unused) continue;
		bool is_png = path.extension() == extension;
		if (!is_png) continue;

		wstring replaced = path.wstring();
		std::replace(replaced.begin(), replaced.end(), L'\\', L'/');
		pPathes->push_back(replaced);
	}
}

void ResourceLoader::LoadTextures(std::vector<wstring>* pPathes)
{
	const std::vector<wstring>& pathes = *pPathes;
	for (auto& path : pathes)
	{
		TextureManager::LoadFromFile(path);
	}
}

void ResourceLoader::LoadAnimations(std::vector<wstring>* pPathes)
{
	const std::vector<wstring>& pathes = *pPathes;
	for (auto& path : pathes)
	{
		AnimationManager::LoadFromFile(path);
	}
}

void ResourceLoader::LoadManagedFiles()
{
	std::vector<wstring> texturePathes;
	std::vector<wstring> animationPathes;
	GetFilePathes(L"../Texture", L".png", &texturePathes);
	GetFilePathes(L"../Animation", L".txt", &animationPathes);
	LoadTextures(&texturePathes);
	LoadAnimations(&animationPathes);

	g_loadComplete = true;
}

bool ResourceLoader::IsLoadComplete()
{
	return g_loadComplete;
}
