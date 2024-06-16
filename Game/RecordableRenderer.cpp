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

	// �Ϲ����� ��������Ʈ �������� �����մϴ�.
	SpriteRenderer::Render();
}

void RecordableRenderer::AfterRender()
{
	if (!m_record) return;

	Sprite* sprite = GetSprite();
	if (!sprite || sprite->m_type != SpriteType::Single) return;

	SpriteRecordInfo info;
	// ��������Ʈ ������Ʈ�� ��ġ, ����, �������� �����մϴ�.
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
	// ��������Ʈ�� ������ ������ �����մϴ�.
	if (m_recordRGBColor) info.color = GetColor();
	else info.color = Color(1, 1, 1, GetColor().a);
	// ��������Ʈ ���� ������ �����մϴ�.
	info.order = GetRenderOrder();
	// ��������Ʈ�� �ĺ��� �� �ִ� Ű�� �����մϴ�.
	info.key = GetSprite()->GetTexture()->GetKey();
	// ��������Ʈ�� additive ���������� �����մϴ�.
	info.additive = IsAdditiveBlending();
	// ��� ���¸� ���ڴ��� �����մϴ�.
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
