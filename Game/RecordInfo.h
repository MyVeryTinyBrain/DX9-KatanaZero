#pragma once

struct SpriteRecordInfo
{
	Vec2 position;
	float angle;
	Vec2 scale;
	Color color;
	wstring key;
	int order;
	bool additive;
	SpriteRecordInfo()
	{
		position = {};
		angle = {};
		scale = Vec2(1, 1);
		color = Color(1, 1, 1, 1);
		key = L"";
		order = {};
		additive = {};
	}
	SpriteRecordInfo(const SpriteRecordInfo& rhs)
	{
		position = rhs.position;
		angle = rhs.angle;
		scale = rhs.scale;
		color = rhs.color;
		key = rhs.key;
		order = rhs.order;
		additive = rhs.additive;
	}
};

struct WorldLineRecordInfo
{
	Vec2 start;
	Vec2 end;
	Color color;
	float width;
	WorldLineRecordInfo()
	{
		start = {};
		end = {};
		color = Color(1, 1, 1, 1);
		width = 1;
	}
	WorldLineRecordInfo(const WorldLineRecordInfo& rhs)
	{
		start = rhs.start;
		end = rhs.end;
		color = rhs.color;
		width = rhs.width;
	}
};

struct CameraRecordInfo
{
	Vec2 position;
	float angle;
	float width;
	float height;
	CameraRecordInfo()
	{
		position = {};
		angle = {};
		width = 1920;
		height = 1080;
	}
	CameraRecordInfo(const CameraRecordInfo& rhs)
	{
		position = rhs.position;
		angle = rhs.angle;
		width = rhs.width;
		height = rhs.height;
	}
};

