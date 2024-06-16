#include "stdafx.h"
#include "RecordableRenderer.h"
#include "Recorder.h"
#include "IRecorderbleRendererCallback.h"

#ifdef SpriteRenderer
#undef SpriteRenderer
#endif

void RecordableRenderer::Awake()
{
	SpriteRenderer::Awake();

	m_recordRGBColor = true;
	m_record = true;
	m_callBack = nullptr;
}

void RecordableRenderer::Render()
{
	auto recorder = Recorder::GetInstance();
	if (recorder && recorder->IsPlaying()) return;

	// 일반적인 스프라이트 렌더링을 수행합니다.
	SpriteRenderer::Render();
}

void RecordableRenderer::AfterRender()
{
	if (!m_record) return;

	Sprite* sprite = GetSprite();
	if (!sprite || sprite->m_type != SpriteType::Single) return;

	SpriteRecordInfo info;
	// 스프라이트 오브젝트의 위치, 각도, 스케일을 저장합니다.
	if (m_callBack)
	{
		info.position = m_callBack->GetRecordPosition();
		info.angle = m_callBack->GetRecordAngle();
		info.scale = m_callBack->GetRecordScale();
	}
	else
	{
		info.position = GetTransform()->GetPosition();
		info.angle = GetTransform()->GetAngle();
		info.scale = GetTransform()->GetScale();
	}
	// 스프라이트에 곱해진 색상을 저장합니다.
	if (m_recordRGBColor) info.color = GetColor();
	else info.color = Color(1, 1, 1, GetColor().a);
	// 스프라이트 렌더 순서를 저장합니다.
	info.order = GetRenderOrder();
	// 스프라이트를 식별할 수 있는 키를 저장합니다.
	info.key = GetSprite()->GetTexture()->GetKey();
	// 스프라이트가 additive 상태인지를 저장합니다.
	info.additive = IsAdditiveBlending();
	// 모든 상태를 레코더에 저장합니다.
	Recorder::GetInstance()->RecordSprite(info);
}

void RecordableRenderer::SetRecordRGBMode(bool enable)
{
	m_recordRGBColor = enable;
}

void RecordableRenderer::SetRecord(bool enable)
{
	m_record = enable;
}
