#pragma once

class IRecorderbleRendererCallback abstract
{
	PUBLIC virtual Vec2 GetRecordPosition() = 0;
	PUBLIC virtual float GetRecordAngle() = 0;
	PUBLIC virtual Vec2 GetRecordScale() = 0;
};

