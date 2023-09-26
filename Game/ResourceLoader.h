#pragma once

class ResourceLoader
{
	PUBLIC static void GetFilePathes(
		const wstring& beginLocalDirectory,
		const wstring& extension,
		std::vector<wstring>* pPathes);
	PUBLIC static void LoadTextures(std::vector<wstring>* pPathes);
	PUBLIC static void LoadAnimations(std::vector<wstring>* pPathes);

	PUBLIC static void LoadManagedFiles();
	PUBLIC static bool IsLoadComplete();
	PRIVATE static bool g_loadComplete;
};

