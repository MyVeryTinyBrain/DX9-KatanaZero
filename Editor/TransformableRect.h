#pragma once

class TransformableRect : public Component
{
	enum SELECTION { SEL_NONE, SEL_LEFT, SEL_TOP, SEL_RIGHT, SEL_BOTTOM, SEL_CENTER };

	COMPONENT_OVERRIDE(Awake);
	COMPONENT_OVERRIDE(Update);
	COMPONENT_OVERRIDE(LateUpdate);
	COMPONENT_OVERRIDE(Render);
	COMPONENT_OVERRIDE(OnDestroy);

	PUBLIC void SetSize(int width, int height);
	PUBLIC int GetWidth() const;
	PUBLIC int GetHeight() const;

	PRIVATE void PickDelete();
	PRIVATE void PickHandle();
	PRIVATE void PutHandle();
	PRIVATE void Handling();

	PRIVATE Vec2 center() const;
	PRIVATE Vec2 left() const;
	PRIVATE Vec2 top() const;
	PRIVATE Vec2 right() const;
	PRIVATE Vec2 bottom() const;

	PRIVATE Transform* m_left;
	PRIVATE Transform* m_top;
	PRIVATE Transform* m_right;
	PRIVATE Transform* m_bottom;
	PRIVATE SELECTION m_selection;

	PRIVATE int m_width;
	PRIVATE int m_height;

	PUBLIC bool m_useLeft;
	PUBLIC bool m_useTop;
	PUBLIC bool m_useRight;
	PUBLIC bool m_useBottom;
	PUBLIC bool m_useCenter;
	PUBLIC bool m_removable;

	PUBLIC Color m_lineColor;

	PRIVATE static TransformableRect* g_select;
};

