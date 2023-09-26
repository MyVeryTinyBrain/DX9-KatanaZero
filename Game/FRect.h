#pragma once

struct FRect
{
	union
	{
		struct
		{
			float left;
			float top;
			float right;
			float bottom;
		};
		struct
		{
			Vec2 leftTop;
			Vec2 rightBottom;
		};
	};

	FRect();

	FRect(float left, float top, float right, float bottom);

	FRect(const Vec2& leftTop, const Vec2& rightBottom);

	float GetWidth() const;

	float GetHeight() const;

	bool Contains(const Vec2& point) const;

	static bool AABB(const FRect& a, const FRect& b);
};

