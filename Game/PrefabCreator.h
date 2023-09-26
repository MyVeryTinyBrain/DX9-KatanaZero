#pragma once

class Cursor;
class TimeController;

class PrefabCreator
{
	PUBLIC static Camera* CreateCamera();
	PUBLIC static void InitalizePhysics();
	PUBLIC static Cursor* CreateCursor();
	PUBLIC static TimeController* CreateTimeController();

	PUBLIC static BoxCollider* CreateGround(const Vec2& position);
	PUBLIC static LineCollider* CreatePlatform(const Vec2& position);
	PUBLIC static RightTriangleCollider* CreateStairs(const Vec2& position);
};

