#include "base_stdafx.h"
#include "LineDevice.h"
#include "Screen.h"
#include "SpriteDevice.h"
#include "CCamera2D.h"

LineDevice* LineDevice::Create(LPDIRECT3DDEVICE9 device)
{
    LPD3DXLINE line;
    if (FAILED(D3DXCreateLine(device, &line)))
        return nullptr;
    return new LineDevice(line);
}

LineDevice::LineDevice(LPD3DXLINE line)
{
    m_line = line;
    m_memoryPool = new Vec2[0];
    m_memoryPoolSize = 0;
}

LineDevice::~LineDevice()
{
    SAFE_DELETE_ARRAY(m_memoryPool);
    SAFE_RELEASE(m_line);
}

void LineDevice::Draw2DLine(const Vec2& a, const Vec2& b, Draw2DLineParameters* pp)
{
    Vec2 v[2] = { a, b };
    if (pp->inWorld)
        WorldToScreen(v, 2);

    float width = pp->width;
    if (pp->inWorld && pp->worldScaleToWidth)
    {
        float cx = fabsf(CCamera2D::GetInstance()->GetScale().x);
        float cy = fabsf(CCamera2D::GetInstance()->GetScale().y);
        if (cx > cy) width /= cx;
        else width /= cy;
    }

    m_line->SetWidth(width);
    m_line->Begin();
    m_line->Draw(v, 2, pp->color);
    m_line->End();
}

void LineDevice::Draw2DRect(const float& l, const float& t, const float& r, const float& b, Draw2DLineParameters* pp)
{
    Draw2DLine({ l, t }, { r, t }, pp);
    Draw2DLine({ l, t }, { l, b }, pp);
    Draw2DLine({ r, t }, { r, b }, pp);
    Draw2DLine({ l, b }, { r, b }, pp);
}

void LineDevice::Draw2DCircle(const Vec2& c, const float& r, Draw2DLineParameters* pp)
{
    constexpr int step = 12;
    auto Vertex = [&](const float& radian) -> Vec2
    {
        return Vec2::Direction(radian) * r + c;
    };
    Vec2 prev = Vertex(0);
    for (int i = 1; i <= step; ++i)
    {
        float percent = float(i) / float(step);
        Vec2 current = Vertex(percent * TAU);
        Draw2DLine(prev, current, pp);
        prev = current;
    }
}

void LineDevice::Draw2DPolygon(const Vec2* v, const DWORD& size, Draw2DLineParameters* pp)
{
    Vec2 prev = v[0];
    for (DWORD i = 1; i <= size; ++i)
    {
        DWORD index = i == size ? 0 : i;
        Vec2 current = v[index];
        Draw2DLine(prev, current, pp);
        prev = current;
    }
}

void LineDevice::Draw2DChain(const Vec2* v, const DWORD& size, Draw2DLineParameters* pp)
{
    Copy(v, size);
    if (pp->inWorld)
        WorldToScreen(m_memoryPool, size);

    m_line->SetWidth(pp->width);
    m_line->Begin();
    m_line->Draw(m_memoryPool, size, pp->color);
    m_line->End();
}

void LineDevice::WorldToScreen(Vec2* v, DWORD size)
{
    Mat4 worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
    Mat4 cameraToScreen = Screen::GetCameraToScreenMatrix();
    Mat4 m = worldToCamera * cameraToScreen;
    for (DWORD i = 0; i < size; ++i)
    {
        Vec2 windowPoint = VEC2_WORLD_TO_WINDOW(v[i]);
        Vec4 wp4 = { windowPoint.x,windowPoint.y, 0, 1 };
        v[i] = wp4 * m;
    }
}

LPDIRECT3DDEVICE9 LineDevice::GetDevice() const
{
    LPDIRECT3DDEVICE9 device;
    m_line->GetDevice(&device);
    return device;
}

LPD3DXLINE LineDevice::GetLine() const
{
    return m_line;
}

void LineDevice::Alloc(const DWORD& size)
{
    if (size <= m_memoryPoolSize) return;
    Vec2* newMemoryPool = new Vec2[size];
    memcpy(newMemoryPool, m_memoryPool, m_memoryPoolSize);
    SAFE_DELETE(m_memoryPool);
    m_memoryPool = newMemoryPool;
    m_memoryPoolSize = size;
}

void LineDevice::Copy(const Vec2* v, const DWORD& size)
{
    Alloc(size);
    memcpy(m_memoryPool, v, size * sizeof(Vec2));
}
