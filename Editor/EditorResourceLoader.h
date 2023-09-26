#pragma once

class EditorResourceLoader
{
	PUBLIC static void GetFilePathes(
		const wstring& beginLocalDirectory,
		const wstring& extension,
		std::vector<wstring>* pPathes);
	PUBLIC static void LoadTextures(std::vector<wstring>* pPathes);
	PUBLIC static void SimpleLoad();
};
