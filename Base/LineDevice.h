#pragma once

struct Draw2DLineParameters
{
	bool inWorld = true; // world or screen
	bool worldScaleToWidth = true;
	float width = 1;
	Color color = Color(0, 1, 0, 1);
};

class LineDevice
{
	PUBLIC static LineDevice* Create(LPDIRECT3DDEVICE9 device);
	PRIVATE LineDevice(LPD3DXLINE line);
	PUBLIC ~LineDevice();

	PUBLIC void Draw2DLine(const Vec2& a, const Vec2& b, Draw2DLineParameters* pp);
	PUBLIC void Draw2DRect(const float& l, const float& t, const float& r, const float& b, Draw2DLineParameters* pp);
	PUBLIC void Draw2DCircle(const Vec2& c, const float& r, Draw2DLineParameters* pp);
	PUBLIC void Draw2DPolygon(const Vec2* v, const DWORD& size, Draw2DLineParameters* pp);
	PUBLIC void Draw2DChain(const Vec2* v, const DWORD& size, Draw2DLineParameters* pp);
	PRIVATE void WorldToScreen(Vec2* v, DWORD size);

	PUBLIC LPDIRECT3DDEVICE9 GetDevice() const;
	PUBLIC LPD3DXLINE GetLine() const;

	PRIVATE void Alloc(const DWORD& size);
	PRIVATE void Copy(const Vec2* v, const DWORD& size);

	PRIVATE LPD3DXLINE m_line;
	PRIVATE DWORD m_memoryPoolSize;
	PRIVATE Vec2* m_memoryPool;
};

