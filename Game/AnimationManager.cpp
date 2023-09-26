#include "stdafx.h"
#include "AnimationManager.h"
#include "Animation.h"

#define INST (g_instance)

IMPLEMENT_SINGLETON(AnimationManager)

IMPLEMENT_SINGLETON_CONSTRUCTOR(AnimationManager)
{

}

IMPLEMENT_SINGLETON_DESTRUCTOR(AnimationManager)
{
	Clear();
}

Animation* AnimationManager::LoadFromFile(const wstring& filePath)
{
    // 삽입 시도를 합니다.
    // 반환하는 페어의 second는 이미 삽입하려는 키가 있는 경우 false, 아닌 경우 true를 반환합니다.
    std::pair<std::map<wstring, Animation*>::iterator, bool> result = INST.m_animations.insert({ filePath, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        // 키가 없는 경우에는 새로 생성하여 삽입 후 반환합니다.
        Animation* animation = Animation::FromFile(filePath);
        result.first->second = animation;
        return animation;
    }
    else
    {
        // 키가 있는 경우에는 이미 존재하는 애니메이션을 반환합니다.
        return result.first->second;
    }
}

Animation* AnimationManager::Insert(const wstring& key, Animation* animation)
{
    std::pair<std::map<wstring, Animation*>::iterator, bool> result = INST.m_animations.insert({ key, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        result.first->second = animation;
        return animation;
    }
    else
    {
        return result.first->second;
    }
}

Animation* AnimationManager::GetAnimation(const wstring& key)
{
	auto it = INST.m_animations.find(key);
	if (it == INST.m_animations.end()) return nullptr;
	return it->second;
}

void AnimationManager::Clear()
{
	for (auto& pair : INST.m_animations)
		SAFE_DELETE(pair.second);
	INST.m_animations.clear();
}
