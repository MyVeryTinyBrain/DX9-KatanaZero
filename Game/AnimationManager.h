#pragma once

class Animation;

class AnimationManager
{
	DECLARE_SINGLETON(AnimationManager)

	PUBLIC static Animation* LoadFromFile(const wstring& filePath);
	PUBLIC static Animation* Insert(const wstring& key, Animation* animation);
	PUBLIC static Animation* GetAnimation(const wstring& key);
	PUBLIC static void Clear();

	PRIVATE std::map<wstring, Animation*> m_animations;
};

