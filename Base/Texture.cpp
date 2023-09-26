#include "base_stdafx.h"
#include "Texture.h"
#include "GraphicDevice.h"

Texture::Texture(const LPDIRECT3DTEXTURE9& texture, const D3DXIMAGE_INFO& imageInfo)
{
	m_imageInfo = imageInfo;
	m_texture = texture;
}

Texture::~Texture()
{
	//SAFE_RELEASE(m_texture);
	m_texture->Release();
}

Texture* Texture::Create(const wstring& filePath)
{
	HRESULT result{};

	// �̹����� ������ �����ɴϴ�.
	// �̹� ������ �ؽ��Ŀ� ���ؼ��� Texture->GetLevelDesc�� ���ؼ��� ������ ������ �� �ֽ��ϴ�.
	// ���⼭�� info�� ��������� �ξ� ���� GetLevelDesc�� ȣ������ �ʾƵ� �˴ϴ�.
	D3DXIMAGE_INFO info{};
	result = D3DXGetImageInfoFromFile(filePath.c_str(), &info);
	if (FAILED(result))
	{
		MESSAGE_BOX(wstring(L"textrue load failed: " + filePath).c_str());
		return nullptr;
	}

	// �ؽ��ĸ� �̹����κ��� �ҷ��ɴϴ�.
	LPDIRECT3DTEXTURE9 texture{};
	result = D3DXCreateTextureFromFileEx
	(
		GraphicDevice::GetInstance()->GetDevice(),
		filePath.c_str(),
		info.Width,
		info.Height,
		info.MipLevels,
		0,
		info.Format,
		D3DPOOL::D3DPOOL_MANAGED, // GPU memory space�� ����ϵ� ������ ��� Ram memory space�� ����մϴ�.
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0, // ����ȭ ��ų �����Դϴ�. 0���� ä�� ����ȭ�� ��ȿȭ�մϴ�. ���⼭�� 0���� ä�� ������� �ʽ��ϴ�.
		nullptr, // �ҷ��� �̹����� D3DXIMAGE_INFO�� ��ȯ�մϴ�. ���⼭�� nullptr�Դϴ�. 
		nullptr,
		&texture
	);
	if (FAILED(result)) return nullptr;

	return new Texture(texture, info);
}

Texture* Texture::Create(const UINT& width, const UINT& height, const D3DXCOLOR& color)
{
	HRESULT hr{};

	LPDIRECT3DTEXTURE9 texture{};
	hr = D3DXCreateTexture
	(
		GraphicDevice::GetInstance()->GetDevice(),
		width,
		height,
		1,
		0,
		D3DFORMAT::D3DFMT_A8R8G8B8,
		D3DPOOL::D3DPOOL_MANAGED,
		&texture
	);
	if (FAILED(hr)) return nullptr;

	D3DXIMAGE_INFO info{};
	info.Width = width;
	info.Height = height;
	info.Depth = 1;
	info.MipLevels = 1;
	info.Format = D3DFORMAT::D3DFMT_A8R8G8B8;
	info.ResourceType = D3DRESOURCETYPE::D3DRTYPE_TEXTURE;
	info.ImageFileFormat = D3DXIFF_PNG;

	D3DLOCKED_RECT pixels;
	texture->LockRect(0, &pixels, 0, D3DLOCK_DISCARD);
	DWORD* colors = (DWORD*)pixels.pBits;
	for (size_t y = 0; y < height; ++y)
	{
		for (size_t x = 0; x < width; ++x)
		{
			int i = y * width + x;
			colors[i] = color;
		}
	}
	texture->UnlockRect(0);

	return new Texture(texture, info);
}

const D3DXIMAGE_INFO& Texture::GetImageInfo() const
{
	return m_imageInfo;
}

LPDIRECT3DTEXTURE9 Texture::GetD3DXTexture() const
{
	return m_texture;
}

Texture::operator LPDIRECT3DTEXTURE9() const
{
	return m_texture;
}

const wstring& Texture::GetKey() const
{
	return m_key;
}
