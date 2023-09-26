#pragma once

#define VEC4_WORLD_TO_WINDOW(VEC4)\
Vec4(VEC4.x, -(VEC4.y), VEC4.z, VEC4.w)

#define VEC4_WINDOW_TO_WORLD(VEC4)\
Vec4(VEC4.x, -(VEC4.y), VEC4.z, VEC4.w)

#define VEC3_WORLD_TO_WINDOW(VEC3)\
Vec3(VEC3.x, -(VEC3.y), VEC3.z)

#define VEC3_WINDOW_TO_WORLD(VEC3)\
Vec3(VEC3.x, -(VEC3.y), VEC3.z)

#define VEC2_WORLD_TO_WINDOW(VEC2)\
Vec2(VEC2.x, -(VEC2.y))

#define VEC2_WINDOW_TO_WORLD(VEC2)\
Vec2(VEC2.x, -(VEC2.y))