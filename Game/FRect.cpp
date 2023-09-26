#include "stdafx.h"
#include "FRect.h"

FRect::FRect() : FRect(0, 0, 0, 0)
{
}
FRect::FRect(float left, float top, float right, float bottom) :
	left(left), top(top), right(right), bottom(bottom)
{
}

FRect::FRect(const Vec2& leftTop, const Vec2& rightBottom) :
	leftTop(leftTop), rightBottom(rightBottom)
{
}

float FRect::GetWidth() const
{
	return right - left;
}

float FRect::GetHeight() const
{
	return top - bottom;
}

bool FRect::Contains(const Vec2& point) const
{
	if (point.x < left) return false;
	if (point.x > right) return false;
	if (point.y < bottom) return false;
	if (point.y > top) return false;
	return true;
}

bool FRect::AABB(const FRect& a, const FRect& b)
{
	if (a.right < b.left || a.left > b.right) return false;
	if (a.top < b.bottom || a.bottom > b.top) return false;
	return true;
}
