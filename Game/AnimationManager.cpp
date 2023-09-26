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
    // ���� �õ��� �մϴ�.
    // ��ȯ�ϴ� ����� second�� �̹� �����Ϸ��� Ű�� �ִ� ��� false, �ƴ� ��� true�� ��ȯ�մϴ�.
    std::pair<std::map<wstring, Animation*>::iterator, bool> result = INST.m_animations.insert({ filePath, nullptr });
    const bool& not_contained = result.second;
    if (not_contained)
    {
        // Ű�� ���� ��쿡�� ���� �����Ͽ� ���� �� ��ȯ�մϴ�.
        Animation* animation = Animation::FromFile(filePath);
        result.first->second = animation;
        return animation;
    }
    else
    {
        // Ű�� �ִ� ��쿡�� �̹� �����ϴ� �ִϸ��̼��� ��ȯ�մϴ�.
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
