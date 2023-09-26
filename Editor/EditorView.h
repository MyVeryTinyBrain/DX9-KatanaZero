
// EditorView.h: CEditorView 클래스의 인터페이스
//

#pragma once

#include "DlgBackground.h"
#include "DlgCollider.h"
#include "DlgParallax.h"
#include "DlgObjectTool.h"
#include "DlgCameraBorderBox.h"
#include "DlgTrigger.h"

class CEditorView : public CView
{
	PRIVATE DlgBackground m_dlgBackground;
	PRIVATE DlgCollider m_dlgCollider;
	PRIVATE DlgParallax m_dlgParallax;
	PRIVATE DlgObjectTool m_dlgObjectTool;
	PRIVATE DlgCameraBorderBox m_dlgCameraBorderBox;
	PRIVATE DlgTrigger m_dlgTrigger;

protected: // serialization에서만 만들어집니다.
	CEditorView() noexcept;
	DECLARE_DYNCREATE(CEditorView)

// 특성입니다.
public:
	CEditorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnBackgroundfront();
	afx_msg void OnBackgroundmiddle();
	afx_msg void OnBackgroundback();
	afx_msg void OnCollider();
	afx_msg void OnParallaxfront();
	afx_msg void OnParallaxmiddle();
	afx_msg void OnParallaxback();
	afx_msg void OnObject();
	afx_msg void OnCameraborder();
	afx_msg void OnTrigger();
};

#ifndef _DEBUG  // EditorView.cpp의 디버그 버전
inline CEditorDoc* CEditorView::GetDocument() const
   { return reinterpret_cast<CEditorDoc*>(m_pDocument); }
#endif

