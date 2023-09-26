#pragma once

enum RENDER_ORDER
{
	RENDER_ORDER_PARALLAX_BACK = -10000,
	RENDER_ORDER_PARALLAX_MIDDLE,
	RENDER_ORDER_PARALLAX_FRONT,

	RENDER_ORDER_BG_BACK = -9020,

	RENDER_ORDER_BG_MIDDLE = -9010,

	RENDER_ORDER_BG_OBJECT = -8000,

	RENDER_ORDER_DOOR = 5000,

	RENDER_ORDER_PIECE = 5500,

	RENDER_ORDER_TURRET_WALL = 6014,
	RENDER_ORDER_TURRET_BODY = 6015,
	RENDER_ORDER_TURRET_HEAD = 6016,
	RENDER_ORDER_TURRET_FRAME = 6017,

	RENDER_ORDER_EFFECT_BACK = 10000,
	RENDER_ORDER_PLATFORM = 10200,
	RENDER_ORDER_EXPLOSIVE_BARREL = 10300,

	RENDER_ORDER_BLACKSCREEN = 50000,

	RENDER_ORDER_THROW_OBJECT = 50010,

	RENDER_ORDER_ENEMY = 66000,
	RENDER_ORDER_BOSS = 66500,
	RENDER_ORDER_PLAYER = 67000,

	RENDER_ORDER_BG_FRONT = 70000,

	RENDER_ORDER_FRONTOBJECT = 90000,
	RENDER_ORDER_BULLET = 100000,
	RENDER_ORDER_THROW_OBJECT_THROWING = 100010,

	RENDER_ORDER_SMOKE = 101000,

	RENDER_ORDER_EFFECT_FRONT = 200000,
	RENDER_ORDER_EFFECT_SWORDEFFECT,
	RENDER_ORDER_EFFECT_BULLET_REFLECT,

	RENDER_ORDER_VISUALIZE_EFFECT = 250000,

	RENDER_ORDER_EFFECT_HIT_LINE = 300000,

	RENDER_ORDER_WORLD_UI = 400000,
	RENDER_ORDER_WORLD_UI_DOWNARROW = 400001,
	RENDER_ORDER_WORLD_UI_GAUGE = 400010,

	RENDER_ORDER_FILL_SCREEN = 500000,
	RENDER_ORDER_FILL_SCREEN_OBJECT = 500010,

	RENDER_ORDER_RECORD = 10000000,

	RENDER_ORDER_EFFECT_SCENE_CHANGE = 20000000,
};