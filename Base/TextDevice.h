#pragma once

enum class TextHAlignment
{
	Left = DT_LEFT,
	Center = DT_CENTER,
	Right = DT_RIGHT,
};

enum class TextVAlignment
{
	Top = DT_TOP,
	Center = DT_VCENTER,
	Bottom = DT_BOTTOM,
};

// 텍스트의 두께입니다.
enum class TextWeight
{
	DontCare = 0,
	Thin = 100,
	Extralight = 200,
	Light = 300,
	Normal = 400,
	Medium = 500,
	Semibold = 600,
	Bold = 700,
	Extrabold = 800,
	Heavy = 900
};

struct WriteTextParameters
{
	wstring fontName = L"Consolas";
	// width  = size * 0.5
	// height = size
	UINT size = 20;
	TextHAlignment hAlignment = TextHAlignment::Left;
	TextVAlignment vAlignment = TextVAlignment::Center;
	TextWeight weight = TextWeight::Heavy;
	Color color = Color(1, 1, 1, 1);
	Vec2 position = {};
	float angle = 0;
	Vec2 scale = { 1,1 };
	bool inWorld = true;
};

class SpriteDevice;

class TextDevice
{
	PUBLIC static TextDevice* Create(SpriteDevice* spriteDevice);
	PRIVATE static D3DXFONT_DESCW CreateInitInfo();
	PRIVATE static LPD3DXFONT CreateD3DFont(SpriteDevice* spriteDevice, const D3DXFONT_DESCW& info);
	PRIVATE TextDevice(SpriteDevice* spriteDevice, LPD3DXFONT font);
	PUBLIC ~TextDevice();

	PRIVATE void SetFont(WriteTextParameters* pp);

	// 텍스트를 출력합니다.
	// 출력한 텍스트의 로컬 공간의 사각 영역을 반환합니다.
	PUBLIC RECT WriteText(const wstring& text, WriteTextParameters* pp);

	// font를 Parameters의 변수에 따라 재설정합니다.
	// 그리고 텍스트를 출력하려는 경우의 로컬 공간의 사각 영역을 반환합니다.
	PUBLIC RECT CalculateRect(const wstring& text, const TextHAlignment& hAlignment, const TextVAlignment& vAlignment);

	PUBLIC const D3DXFONT_DESCW& GetFontInfo() const;
	PUBLIC LPD3DXFONT GetFont() const;
	PUBLIC LPDIRECT3DDEVICE9 GetDevice() const;

	PRIVATE SpriteDevice* m_spriteDevice;
	PRIVATE LPD3DXFONT m_font;
	PRIVATE D3DXFONT_DESCW m_info;
};

