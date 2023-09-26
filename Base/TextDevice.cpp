#include "base_stdafx.h"
#include "TextDevice.h"
#include "SpriteDevice.h"
#include "CCamera2D.h"
#include "Screen.h"

#define SET_SIZE(INFO, SIZE)	\
INFO.Width = SIZE/2;			\
INFO.Height = SIZE;

TextDevice* TextDevice::Create(SpriteDevice* spriteDevice)
{
	if (!spriteDevice) return nullptr;
	D3DXFONT_DESCW info = CreateInitInfo();
	LPD3DXFONT font = CreateD3DFont(spriteDevice, info);
	if (!font) return nullptr;
	return new TextDevice(spriteDevice, font);
}

D3DXFONT_DESCW TextDevice::CreateInitInfo()
{
	D3DXFONT_DESCW info;
	ZERO_MEM(info);
	WriteTextParameters p;
	info.CharSet = HANGEUL_CHARSET;
	SET_SIZE(info, p.size);
	info.Weight = UINT(p.weight);
	wcscpy_s(info.FaceName, p.fontName.c_str());
	return info;
}

LPD3DXFONT TextDevice::CreateD3DFont(SpriteDevice* spriteDevice, const D3DXFONT_DESCW& info)
{
	LPD3DXFONT font;
	if (FAILED(D3DXCreateFontIndirectW(spriteDevice->GetDevice(), &info, &font)))
		return nullptr;
	return font;
}

TextDevice::TextDevice(SpriteDevice* spriteDevice, LPD3DXFONT font)
{
	m_spriteDevice = spriteDevice;
	font->GetDescW(&m_info);
	m_font = font;
}

TextDevice::~TextDevice()
{
	SAFE_RELEASE(m_font);
}

void TextDevice::SetFont(WriteTextParameters* pp)
{
	// ��Ʈ ������ �ſ� ���̽��ϴ�.
	// ������� �ʿ䰡 ���ٸ� �����մϴ�.
	if (m_info.Height == pp->size &&
		m_info.Weight == UINT(pp->weight) &&
		m_info.FaceName == pp->fontName)
		return;

	D3DXFONT_DESCW info = m_info;
	SET_SIZE(info, pp->size);
	info.CharSet = HANGEUL_CHARSET;
	info.Weight = UINT(pp->weight);
	wcscpy_s(info.FaceName, pp->fontName.c_str());

	LPD3DXFONT font = CreateD3DFont(m_spriteDevice, info);
	if (!font) return;
	SAFE_RELEASE(m_font);

	m_font = font;
	m_info = info;
}

RECT TextDevice::WriteText(const wstring& text, WriteTextParameters* pp)
{
	Mat4 m;
	if (pp->inWorld)
	{
		Vec2 position = VEC2_WORLD_TO_WINDOW(pp->position);
		Mat4 localToWorld = Mat4::SRT(pp->scale, { 0, 0, pp->angle * DEG2RAD }, position);
		Mat4 worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
		Mat4 cameraToScreen = Screen::GetCameraToScreenMatrix();
		m = localToWorld * worldToCamera * cameraToScreen;
	}
	else
	{
		Mat4 localToWorld = Mat4::SRT(pp->scale, { 0, 0, -pp->angle * DEG2RAD }, pp->position);
		m = localToWorld;
	}
	// �ؽ�Ʈ Ʈ�������� �����մϴ�.
	m_spriteDevice->GetSprite()->SetTransform(&m);
	// ��Ʈ�� �缳���մϴ�.
	SetFont(pp);
	// �簢�� ������ ����մϴ�.
	RECT rect = CalculateRect(text.c_str(), pp->hAlignment, pp->vAlignment);
	// �ؽ�Ʈ�� ���ϴ�.
	m_font->DrawTextW(m_spriteDevice->GetSprite(), text.c_str(), text.length(), &rect, DWORD(pp->hAlignment) | DWORD(pp->vAlignment), pp->color);
	return rect;
}

RECT TextDevice::CalculateRect(const wstring& text, const TextHAlignment& hAlignment, const TextVAlignment& vAlignment)
{
	RECT rt = {};
	// DT_CALCRECT: m_font�� �׸����� �ؽ�Ʈ�� ������ŭ�� RECT�� ����� ��ȯ�ϵ��� �մϴ�.
	//				�̶� �ؽ�Ʈ�� �׸����� �ʽ��ϴ�.
	m_font->DrawTextW(m_spriteDevice->GetSprite(), text.c_str(), text.length(), &rt, DWORD(hAlignment) | DWORD(vAlignment) | DT_CALCRECT, 0);
	return rt;
}

const D3DXFONT_DESCW& TextDevice::GetFontInfo() const
{
	return m_info;
}

LPD3DXFONT TextDevice::GetFont() const
{
	return m_font;
}

LPDIRECT3DDEVICE9 TextDevice::GetDevice() const
{
	LPDIRECT3DDEVICE9 device;
	m_font->GetDevice(&device);
	return device;
}
