#pragma once

template <typename T, typename AnyType>
inline T cast(AnyType* ptr)
{
	return (T)(ptr);
}