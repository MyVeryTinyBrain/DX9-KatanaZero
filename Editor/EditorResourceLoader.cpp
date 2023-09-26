#include "editor_stdafx.h"
#include "EditorResourceLoader.h"

namespace fs = std::filesystem;
using std::stack;
using std::vector;

void EditorResourceLoader::GetFilePathes(
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

void EditorResourceLoader::LoadTextures(std::vector<wstring>* pPathes)
{
	const std::vector<wstring>& pathes = *pPathes;
	for (auto& path : pathes)
	{
		TextureManager::LoadFromFile(path);
	}
}

void EditorResourceLoader::SimpleLoad()
{
	std::vector<wstring> texturePathes;
	GetFilePathes(L"../Texture", L".png", &texturePathes);
	LoadTextures(&texturePathes);
}
