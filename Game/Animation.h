#pragma once

class Animation
{
	PUBLIC Animation();
	PUBLIC ~Animation();
	PUBLIC static Animation* JsonToAnimation(const string& json);
	PUBLIC static Animation* FromFile(const wstring& filePath);

	PUBLIC void AddSprite(const wstring& textureKey);
	PUBLIC void ClearSprites();

	PUBLIC size_t GetSpriteCount() const;
	PUBLIC float GetMaximumTime() const;
	PUBLIC float GetInterval() const;
	PUBLIC bool IsLoop() const;

	PUBLIC bool IndexOf(int index, Sprite** ppSprite) const;
	PUBLIC bool TimeOf(float time, Sprite** ppSprite) const;
	PUBLIC bool IsEnd(float time) const;
	PUBLIC const Vec2& GetAdjust() const;

	PUBLIC const wstring& GetKey() const;

	PRIVATE std::vector<Sprite*> m_sprites;
	PRIVATE float m_interval;
	PRIVATE bool m_loop;
	PRIVATE Vec2 m_adjust;
	PRIVATE wstring m_key;
};

