#pragma once

enum DebugDrawFlag
{
	DebugDrawTrue = false,
	DebugDrawFalse = false,
};

enum DebugDraw
{
	DRAW_STAGE = DebugDrawTrue,
	DRAW_OBJECT = DebugDrawTrue,
	DRAW_CHARACTER = DebugDrawTrue,
	DRAW_TRIGGER = DebugDrawTrue,
	DRAW_TRIGGER_NAME = DebugDrawTrue,
	DRAW_RAY = DebugDrawTrue,
};