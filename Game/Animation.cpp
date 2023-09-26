#include "stdafx.h"
#include "Animation.h"
#include <fstream>

Animation::Animation()
{
	m_interval = 0.05f;
	m_loop = false;
}

Animation::~Animation()
{
	ClearSprites();
}

Animation* Animation::JsonToAnimation(const string& json)
{
	Json::Value root;

	JSONCPP_STRING err;
	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	if (!reader->parse(json.c_str(), json.c_str() + json.length(), &root, &err))
	{
		return nullptr;
	}

	Animation* animation = new Animation;
	animation->m_interval = root["interval"].asFloat();
	animation->m_loop = root["loop"].asBool();
	float adjust_x = root["adjust_x"].asFloat();
	float adjust_y = root["adjust_y"].asFloat();
	animation->m_adjust = Vec2(adjust_x, adjust_y);
	Json::Value sprites = root["sprites"];
	for (size_t i = 0; i < sprites.size(); ++i)
	{
		string spritePath = sprites[int(i)].asString() + ".png";
		wstring wSpritePath;
		wSpritePath.assign(spritePath.begin(), spritePath.end());
		Sprite* sprite = new Sprite(wSpritePath);
		if (!sprite->GetTexture()->GetD3DXTexture())
		{
			wstring msg = L"sprite missing when animation creation: " + wSpritePath;
			MESSAGE_BOX(msg.c_str());
		}
		animation->m_sprites.push_back(sprite);
	}

	return animation;
}

Animation* Animation::FromFile(const wstring& filePath)
{
	Animation* animation = nullptr;
	std::ifstream stream;
	stream.open(filePath, std::ios_base::in);
	if (stream.is_open())
	{
		string full_str;
		while (!stream.eof())
		{
			string str;
			std::getline(stream, str, '\0');
			full_str += str;
		}
		stream.close();
		animation = JsonToAnimation(full_str);
		if (animation)
		{
			animation->m_key = filePath;
		}
		else
		{
			wstring msg = L"animation format error: " + filePath;
			MESSAGE_BOX(msg.c_str());
		}
	}
	else
	{
		wstring msg = L"animation missing: " + filePath;
		MESSAGE_BOX(msg.c_str());
	}
	return animation;
}

void Animation::AddSprite(const wstring& textureKey)
{
	Sprite* sprite = new Sprite(textureKey);
	m_sprites.push_back(sprite);
}

void Animation::ClearSprites()
{
	for (auto& sprite : m_sprites)
		SAFE_DELETE(sprite);
	m_sprites.clear();
}

size_t Animation::GetSpriteCount() const
{
	return m_sprites.size();
}

float Animation::GetMaximumTime() const
{
	return m_interval * m_sprites.size();
}

float Animation::GetInterval() const
{
	return m_interval;
}

bool Animation::IsLoop() const
{
	return m_loop;
}

bool Animation::IndexOf(int index, Sprite** ppSprite) const
{
	*ppSprite = nullptr;

	int spriteSize = int(m_sprites.size());
	index %= spriteSize;

	if (!m_loop)
	{
		if (index >= spriteSize) return false;
		if (index < 0) return false;
	}
	else
	{
		if (index >= spriteSize) index %= spriteSize;
		if (index < 0) index = spriteSize + (index % spriteSize);
	}

	*ppSprite = m_sprites[index];
	return true;
}

bool Animation::TimeOf(float time, Sprite** ppSprite) const
{
	*ppSprite = nullptr;

	int spriteSize = int(m_sprites.size());
	float maxTime = float(spriteSize) * m_interval;
	float percent = fmodf(time / maxTime, 1.0f);
	float fIndex = percent * float(spriteSize);
	int index = int(fIndex);

	if (!m_loop)
	{
		if (time >= maxTime)
		{
			*ppSprite = m_sprites.back();
			return false;
		}
		if (time < 0)
		{
			*ppSprite = m_sprites.front();
			return false;
		}
	}
	else
	{
		if (index >= spriteSize) index %= spriteSize;
		if (index < 0) index = spriteSize + (index % spriteSize);
	}

	*ppSprite = m_sprites[index];
	return true;
}

bool Animation::IsEnd(float time) const
{
	int spriteSize = int(m_sprites.size());
	float maxTime = float(spriteSize) * m_interval;
	float percent = fmodf(time / maxTime, 1.0f);
	float fIndex = percent * float(spriteSize);
	int index = int(fIndex);

	if (time >= maxTime) return true;
	return false;
}

const Vec2& Animation::GetAdjust() const
{
	return m_adjust;
}

const wstring& Animation::GetKey() const
{
	return m_key;
}
