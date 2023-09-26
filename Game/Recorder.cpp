#include "stdafx.h"
#include "Recorder.h"
#include "TimeController.h"
#include "UI.h"
#include "StageScene.h"
#include "GameRenderOrder.h"
#include "SoundMgr.h"
#include "SceneChangeEffect.h"
#include "PlayerCamera.h"
#include "PlayerController.h"

#define TO_WINDOW_ANGLE (-1)

Recorder* Recorder::g_instance = nullptr;

void Recorder::Awake()
{
    m_renderOrder = RENDER_ORDER_RECORD;

	g_instance = this;

	m_renderOrder = INT_MAX;
	m_afterRenderOrder = INT_MAX;

    m_sprNoise[0] = new Sprite(L"../Texture/effect/used/noise/spr_static_0.png");
    m_sprNoise[1] = new Sprite(L"../Texture/effect/used/noise/spr_static_1.png");
    m_sprNoise[2] = new Sprite(L"../Texture/effect/used/noise/spr_static_2.png");
    m_sprNoise[3] = new Sprite(L"../Texture/effect/used/noise/spr_static_3.png");
    m_sprWhite = new Sprite(L"../Texture/effect/used/white/spr_white.png");

    m_canChangeNextSceneWhenPlay = false;
}

void Recorder::Render()
{
	if (!m_playing) return;

    if (m_playingReverse && Input::GetKeyDown(Key::LeftMouse))
		Skip();

	if (m_playingReverse &&
		m_canReloadSceneWhenPlayReverse &&
		m_cursor == GetFirstKeyFromCameraRecords())
	{
		StageScene* stageScene = dynamic_cast<StageScene*>(SceneManager::GetCurrentScene());
		StageScene* reloadScene = dynamic_cast<StageScene*>(stageScene->Copy());
		reloadScene->DontUseFadeOutEffect();
		SceneManager::ChangeScene(reloadScene);
	}

	if (!m_playingReverse)
	{
		if (m_canChangeNextSceneWhenPlay &&
			Input::GetKeyDown(Key::RightMouse) &&
			!SceneChangeEffect::GetInstance()->IsFading())
		{
			StageScene* stageScene = dynamic_cast<StageScene*>(SceneManager::GetCurrentScene());
			if (stageScene)
			{
                stageScene->ChangeToNextScene();
            }
        }
        else if (Input::GetKeyDown(Key::LeftMouse))
        {
            if (m_pause)
            {
                Resume();
                SetPlayStateText(L"재생");
            }
            else
            {
                Pause();
                SetPlayStateText(L"일시정지");
            }
        }
        else if (Input::GetKeyDown(Key::A) || Input::GetKeyDown(Key::D))
        {
            int iSpeed = int(roundf(m_playSpeed));
            if (Input::GetKeyDown(Key::A))
            {
                iSpeed -= 1;
                if (iSpeed < -2) iSpeed = -2;
                else if (iSpeed == 0) iSpeed = -1;
                Resume();
                if (iSpeed == 1) SetPlayStateText(L"재생");
                else if (iSpeed == 2) SetPlayStateText(L"빠르게 재생");
                m_playSpeed = float(iSpeed);
            }
            else if (Input::GetKeyDown(Key::D))
            {
                iSpeed += 1;
                if (iSpeed > 2) iSpeed = 2;
                else if (iSpeed == 0) iSpeed = 1;
                Resume();
                if (iSpeed == -1) SetPlayStateText(L"되감기");
                else if (iSpeed == -2) SetPlayStateText(L"빠르게 되감기");
                m_playSpeed = float(iSpeed);
            }
            if (iSpeed == 1) SetPlayStateText(L"재생");
            else if (iSpeed == 2) SetPlayStateText(L"빠르게 재생");
            if (iSpeed == -1) SetPlayStateText(L"되감기");
            else if (iSpeed == -2) SetPlayStateText(L"빠르게 되감기");
            m_playSpeed = float(iSpeed);
        }
    }

    DrawBackgroundColor();

    float cameraKey = GetClosetKeyFromCameraRecords(m_cursor);
    if (cameraKey != -1)
    {
        CameraRecordInfo* camInfo = m_cameraPosition[cameraKey];
        CCamera2D::GetInstance()->SetPosition(camInfo->position);
        CCamera2D::GetInstance()->SetAngle(camInfo->angle);
        CCamera2D::GetInstance()->SetWidth(camInfo->width);
        CCamera2D::GetInstance()->SetHeight(camInfo->height);
    }

    float spriteKey = GetClosetKeyFromSpriteRecords(m_cursor);
    if (spriteKey != -1)
    {
        for (auto& value : m_spriteRecords[spriteKey])
        {
            DrawSprite(value);
        }
    }

    float lineKey = GetClosetKeyFromLineRecords(m_cursor);
    if (lineKey != -1 && fabsf(lineKey - m_cursor) <= 0.1f)
    {
        for (auto& value : m_lineRecords[lineKey])
        {
            DrawLine(value);
        }
    }

    if (m_playingReverse && !m_isEndPlay && (cameraKey == -1.0f || cameraKey == GetFirstKeyFromCameraRecords()))
    {
        m_isEndPlay = true;
        OnEndPlayReverse();
    }

    DrawNoise();

    WritePlayStateText();

    if (!m_pause)
        m_cursor += Time::GetUnscaledDeltaTime() * m_playSpeed * (m_playingReverse ? -1.0f : +1.0f);
    if (m_cursor < GetFirstKeyFromCameraRecords()) m_cursor = GetFirstKeyFromCameraRecords();
    else if (m_cursor > GetLastKeyFromCameraRecords()) m_cursor = GetLastKeyFromCameraRecords();
}

void Recorder::AfterRender()
{
	if (m_playing) return;

    std::sort(
        m_spriteRecords[m_elapsed].begin(),
        m_spriteRecords[m_elapsed].end(),
        [](SpriteRecordInfo* left, SpriteRecordInfo* right)
        {
            return left->order < right->order;
        });

    CameraRecordInfo* cameraInfo = new CameraRecordInfo;
    Vec2 targetCamPos = PlayerController::GetInstance()->GetTransform()->GetPosition();
    Vec2 camSize = Vec2(CCamera2D::GetInstance()->GetWidth(), CCamera2D::GetInstance()->GetHeight());
    Vec2 camPos = PlayerCamera::GetInstance()->CalculateFitInBorderPosition(targetCamPos, camSize);
    cameraInfo->position = camPos; /*CCamera2D::GetInstance()->GetPosition();*/
    cameraInfo->angle = CCamera2D::GetInstance()->GetAngle();
    cameraInfo->width = CCamera2D::GetInstance()->GetWidth();
    cameraInfo->height = CCamera2D::GetInstance()->GetHeight();

    auto it = m_cameraPosition.find(m_elapsed);
    if (it != m_cameraPosition.end())
    {
        SAFE_DELETE(cameraInfo);
    }
    else
    {
        m_cameraPosition[m_elapsed] = cameraInfo;
    }

	m_elapsed += Time::GetDeltaTime();
}

void Recorder::OnDestroy()
{
	if (g_instance == this)
		g_instance = nullptr;

	for (auto& v : m_spriteRecords)
	{
		for (auto& value : v.second)
			SAFE_DELETE(value);
		v.second.clear();
	}
	m_spriteRecords.clear();

	for (auto& v : m_lineRecords)
	{
		for (auto& value : v.second)
			SAFE_DELETE(value);
		v.second.clear();
	}
	m_spriteRecords.clear();

	for (auto& value : m_cameraPosition)
		SAFE_DELETE(value.second);
	m_cameraPosition.clear();

    for (auto& spr : m_sprNoise)
        SAFE_DELETE(spr);
    SAFE_DELETE(m_sprWhite);
}

Recorder* Recorder::GetInstance()
{
	return g_instance;
}

void Recorder::RecordSprite(const SpriteRecordInfo& info)
{
	if (m_playing) return;

	m_spriteRecords[m_elapsed].push_back(new SpriteRecordInfo(info));
}

void Recorder::RecordLine(const WorldLineRecordInfo& info)
{
	if (m_playing) return;

	m_lineRecords[m_elapsed].push_back(new WorldLineRecordInfo(info));
}

void Recorder::Play()
{
    PlayerCamera::GetInstance()->Stop();

	m_playing = true;
    m_isEndPlay = false;
	m_cursor = 0;
    m_playSpeed = 1.0f;
    m_showPlayStateText = true;

    TimeController::GetInstance()->Stop();

    m_cameraKeys.clear();
    for (auto& pair : m_cameraPosition)
        m_cameraKeys.push_back(pair.first);

    m_spriteKeys.clear();
    for (auto& pair : m_spriteRecords)
        m_spriteKeys.push_back(pair.first);

    m_lineKeys.clear();
    for (auto& pair : m_lineRecords)
        m_lineKeys.push_back(pair.first);

    UI::GetInstance()->SetEnable(false);

    SetPlayStateText(L"재생");
}

void Recorder::SetSpeed(float speed)
{
    m_playSpeed = speed;
}

void Recorder::PlayReverse()
{
    bool isPlaying = m_playing;
    float playingCursor = m_cursor;

    Play();

    m_playingReverse = true;
    m_playSpeed = 2.0f;
    m_showPlayStateText = false;
    
    if (isPlaying) m_cursor = playingCursor;
    else m_cursor = GetLastKeyFromCameraRecords();

    SetPlayStateText(L"되감기");

    CSoundMgr::Get_Instance()->PlaySound(L"sound_rewind.wav", CSoundMgr::CHANNELID::RECORDER);
}

bool Recorder::IsPlaying() const
{
    return m_playing;
}

bool Recorder::IsPlayingReverse() const
{
    return m_playingReverse;
}

void Recorder::Skip()
{
    m_playSpeed = 10;
}

void Recorder::Pause()
{
    m_pause = true;
}

void Recorder::Resume()
{
    m_pause = false;
}

void Recorder::SetCanChangeNextSceneWhenPlay(bool enable)
{
    m_canChangeNextSceneWhenPlay = enable;
}

void Recorder::SetCanReloadSceneWhenPlayReverse(bool enable)
{
    m_canReloadSceneWhenPlayReverse = enable;
}

float Recorder::GetClosetKeyFromCameraRecords(float value)
{
    if (value <= GetFirstKeyFromCameraRecords()) return GetFirstKeyFromCameraRecords();
    if (value >= GetLastKeyFromCameraRecords()) return GetLastKeyFromCameraRecords();
    auto const it = std::lower_bound(m_cameraKeys.begin(), m_cameraKeys.end(), value);
    if (it == m_cameraKeys.end())
        return GetLastKeyFromCameraRecords();
    return *it;
}

float Recorder::GetClosetKeyFromSpriteRecords(float value)
{
    if (value <= GetFirstKeyFromSpriteRecords()) return GetFirstKeyFromSpriteRecords();
    if (value >= GetLastKeyFromSpriteRecords()) return GetLastKeyFromSpriteRecords();
    auto const it = std::lower_bound(m_spriteKeys.begin(), m_spriteKeys.end(), value);
    if (it == m_spriteKeys.end())
        return GetLastKeyFromSpriteRecords();
    return *it;
}

float Recorder::GetClosetKeyFromLineRecords(float value)
{
    if (value <= GetFirstKeyFromLineRecords()) return GetFirstKeyFromLineRecords();
    if (value >= GetLastKeyFromLineRecords()) return GetLastKeyFromLineRecords();
    auto const it = std::lower_bound(m_lineKeys.begin(), m_lineKeys.end(), value);
    if (it == m_lineKeys.end())
        return GetLastKeyFromLineRecords();
    return *it;
}

void Recorder::DrawSprite(SpriteRecordInfo* info)
{
    Vec4 position = info->position;
    position.z = 0.5f; //////////////////////////////////////////////////////////////////////////////////////
    position.w = 1.0f;
    const Mat4& localToWorld = Mat4::SRT(
        info->scale,
        { 0,0,info->angle * DEG2RAD * TO_WINDOW_ANGLE },
        VEC4_WORLD_TO_WINDOW(position)
    );
    const Mat4& worldToCamera = CCamera2D::GetInstance()->GetWorldToCameraMatrix();
    const Mat4& cameraToScreen = Screen::GetCameraToScreenMatrix();

    // blending
    // https://gamedev.stackexchange.com/questions/29543/how-to-mix-textures-in-directx
    if (info->additive)
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

    auto tex = TextureManager::GetTexture(info->key);
    GraphicDevice::GetSpriteDevice()->DrawSingleTextrue(tex, localToWorld * worldToCamera * cameraToScreen, info->color);

    if (info->additive)
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

void Recorder::DrawLine(WorldLineRecordInfo* info)
{
    auto line = GraphicDevice::GetLineDevice();
    Draw2DLineParameters param;
    param.color = info->color;
    param.width = info->width;
    line->Draw2DLine(info->start, info->end, &param);
}

void Recorder::DrawNoise()
{
    constexpr float noiseInterval = 0.03f;
    float fNoiseIndex = fmodf(m_noiseElapsed, noiseInterval * 4.0f) / noiseInterval;
    size_t noiseIndex = size_t(fNoiseIndex);

    auto sprNoise = m_sprNoise[noiseIndex];
    float screenWidth = float(Screen::GetWidth());
    float screenHeight = float(Screen::GetHeight());
    float noiseWidth = sprNoise->GetSize().x;
    float noiseHeight = sprNoise->GetSize().y;
    float xRatio = screenWidth / noiseWidth;
    float yRatio = screenHeight / noiseHeight;
    Mat4 matNoise = Mat4::SRT(Vec3(xRatio, yRatio, 1), Vec3::zero(), Vec3(screenWidth * 0.5f, screenHeight * 0.5f, 0.5f));//////////////////////////////////////////////////////////////////////////////////////
    GraphicDevice::GetSpriteDevice()->DrawSprite(sprNoise, matNoise, Color(1, 1, 1, 0.2f));

    m_noiseElapsed += Time::GetUnscaledDeltaTime();
}

void Recorder::DrawBackgroundColor()
{
    float screenWidth = float(Screen::GetWidth());
    float screenHeight = float(Screen::GetHeight());
    float sprWidth = m_sprWhite->GetSize().x;
    float sprHeight = m_sprWhite->GetSize().y;
    float xRatio = screenWidth / sprWidth;
    float yRatio = screenHeight / sprHeight;
    Mat4 matWhite = Mat4::SRT(Vec3(xRatio, yRatio, 1), Vec3::zero(), Vec3(screenWidth * 0.5f, screenHeight * 0.5f, 0.5f));//////////////////////////////////////////////////////////////////////////////////////
    GraphicDevice::GetSpriteDevice()->DrawSprite(m_sprWhite, matWhite, GraphicDevice::GetBackgroundColor());
}

void Recorder::WritePlayStateText()
{
    if (!m_showPlayStateText) return;

    float size = float(Screen::GetWidth()) * 0.065f;
    if (m_playStateTextAlpha > 0)
    {
        WriteTextParameters param;
        param.vAlignment = TextVAlignment::Top;
        param.hAlignment = TextHAlignment::Left;
        param.inWorld = false;
        param.color = Color(1, 1, 1, m_playStateTextAlpha);
        param.position = Vec2(0, 0);
        param.fontName = L"돋움";
        param.size = UINT(size);
        GraphicDevice::GetTextDevice()->WriteText(m_playStateText, &param);
        m_playStateTextAlpha -= Time::GetUnscaledDeltaTime();
    }
    {
        WriteTextParameters param;
        param.vAlignment = TextVAlignment::Bottom;
        param.hAlignment = TextHAlignment::Right;
        param.inWorld = false;
        param.color = Color(1, 1, 1, 1);
        param.position = Vec2(float(Screen::GetWidth()), float(Screen::GetHeight()));
        param.fontName = L"돋움";
        param.size = UINT(size);
        GraphicDevice::GetTextDevice()->WriteText(ToTimeFormat(m_cursor) + L'/' + ToTimeFormat(GetLastKeyFromCameraRecords()), &param);
    }
}

void Recorder::SetPlayStateText(const wstring& text)
{
    m_playStateText = text;
    m_playStateTextAlpha = 1.0f;
}

wstring Recorder::ToTimeFormat(float secondsValue)
{
    int seconds = int(roundf(secondsValue));
    int minute = int(seconds / 60.0f);

    wstring wMinute = std::to_wstring(minute);
    if (wMinute.length() == 1) wMinute = L'0' + wMinute;

    wstring wSeconds = std::to_wstring(seconds);
    if (wSeconds.length() == 1) wSeconds = L'0' + wSeconds;

    return wMinute + L':' + wSeconds;
}

float Recorder::GetFirstKeyFromCameraRecords() const
{
    int begin = 10;
    for (int i = begin; i >= 0; --i)
    {
        if (m_cameraKeys.size() > i + 1) return m_cameraKeys[i];
    }

    return -1.0f;
}

float Recorder::GetFirstKeyFromSpriteRecords() const
{
    int begin = 10;
    for (int i = begin; i >= 0; --i)
    {
        if (m_spriteKeys.size() > i + 1) return m_spriteKeys[i];
    }

    return -1.0f;
}

float Recorder::GetFirstKeyFromLineRecords() const
{
    int begin = 10;
    for (int i = begin; i >= 0; --i)
    {
        if (m_lineKeys.size() > i + 1) return m_lineKeys[i];
    }

    return -1.0f;
}

float Recorder::GetLastKeyFromCameraRecords() const
{
    if (m_cameraKeys.size() == 0) return -1;
    return m_cameraKeys.back();
}

float Recorder::GetLastKeyFromSpriteRecords() const
{
    if (m_spriteKeys.size() == 0) return -1;
    return m_spriteKeys.back();
}

float Recorder::GetLastKeyFromLineRecords() const
{
    if (m_lineKeys.size() == 0) return -1;
    return m_lineKeys.back();
}
