#include "base_stdafx.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "GraphicDevice.h"
#include "SpriteDevice.h"
#include "CCamera2D.h"
#include "Screen.h"

#define TO_WINDOW_ANGLE (-1)

void SpriteRenderer::Awake()
{
    m_sprite = nullptr;
    m_color = Color(1, 1, 1, 1);
    m_additiveBlending = false;
}

void SpriteRenderer::Render()
{
    if (!m_sprite) return;

    Transform* transform = GetTransform();
    Vec4 position = transform->GetPosition();
    position.z = 0.5f;
    position.w = 1.0f;
    const Mat4& localToWorld = Mat4::SRT(
        transform->GetScale(), 
        { 0,0,transform->GetAngle() * DEG2RAD * TO_WINDOW_ANGLE },
        VEC4_WORLD_TO_WINDOW(position)
    );
    const Mat4& worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
    const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();

    // blending
    // https://gamedev.stackexchange.com/questions/29543/how-to-mix-textures-in-directx
    if (m_additiveBlending)
    {
        GraphicDevice::GetSpriteDevice()->EndRender();
        GraphicDevice::GetSpriteDevice()->BeginRender();

        // 포인트 샘플링
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

        // Additive Blending
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    GraphicDevice::GetSpriteDevice()->DrawSprite(m_sprite, localToWorld * worldToCamera * cameraToScreen, m_color);

    if (m_additiveBlending)
    {
        GraphicDevice::GetSpriteDevice()->EndRender();
        GraphicDevice::GetSpriteDevice()->BeginRender();

        // 포인트 샘플링
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        GraphicDevice::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

        // 기본 블렌딩
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        GraphicDevice::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }
}

void SpriteRenderer::OnDestroy()
{
}

void SpriteRenderer::SetSprite(Sprite* sprite)
{
    m_sprite = sprite;
}

Sprite* SpriteRenderer::GetSprite() const
{
    return m_sprite;
}

void SpriteRenderer::SetColor(const Color& color)
{
    m_color = color;
}

const Color& SpriteRenderer::GetColor() const
{
    return m_color;
}

void SpriteRenderer::SetRenderOrder(int order)
{
    m_renderOrder = order; 
}

void SpriteRenderer::SetAdditiveBlending(bool enable)
{
    m_additiveBlending = enable;
}

bool SpriteRenderer::IsAdditiveBlending() const
{
    return m_additiveBlending;
}
