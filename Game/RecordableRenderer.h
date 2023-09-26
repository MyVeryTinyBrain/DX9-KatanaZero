#pragma once

#ifdef SpriteRenderer
#undef SpriteRenderer
#endif

class IRecorderbleRendererCallback;

class RecordableRenderer : public SpriteRenderer
{
	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(AfterRender);

	PUBLIC void SetRecordRGBMode(bool enable);
	PUBLIC void SetRecord(bool enable);

	PRIVATE bool m_recordRGBColor;
	PRIVATE bool m_record;
	PUBLIC IRecorderbleRendererCallback* m_callBack;
};

