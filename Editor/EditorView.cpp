
// EditorView.cpp: CEditorView 클래스의 구현
//

#include "editor_stdafx.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include "EditorDoc.h"
#include "EditorView.h"
#include "MainFrm.h"

#ifdef new
#undef new
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ==========================================================
#include "editor_main.h"
#include "EditorResourceLoader.h"
#include "EditorManager.h"
// ==========================================================

// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_FILE_NEW, &CEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CEditorView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CEditorView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CEditorView::OnFileSaveAs)
	ON_COMMAND(ID_Collider, &CEditorView::OnCollider)
	ON_COMMAND(ID_ParallaxFront, &CEditorView::OnParallaxfront)
	ON_COMMAND(ID_ParallaxMiddle, &CEditorView::OnParallaxmiddle)
	ON_COMMAND(ID_ParallaxBack, &CEditorView::OnParallaxback)
	ON_COMMAND(ID_Object, &CEditorView::OnObject)
	ON_COMMAND(ID_CameraBorder, &CEditorView::OnCameraborder)
	ON_COMMAND(ID_Trigger, &CEditorView::OnTrigger)
	ON_COMMAND(ID_BackgroundFront, &CEditorView::OnBackgroundfront)
	ON_COMMAND(ID_BackgroundMiddle, &CEditorView::OnBackgroundmiddle)
	ON_COMMAND(ID_BackgroundBack, &CEditorView::OnBackgroundback)
END_MESSAGE_MAP()

// CEditorView 생성/소멸

CEditorView::CEditorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEditorView::~CEditorView()
{
	editor_main::Release();
}

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CEditorView 그리기

void CEditorView::OnDraw(CDC* /*pDC*/)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CEditorView 인쇄

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CEditorView 진단

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CEditorView 메시지 처리기


void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// ================================================================================================
	if (GraphicDevice::GetInstance()->IsInitialized())
	{
		Screen::GetInstance()->UpdateSize(cx, cy);
		GraphicDevice::GetInstance()->Reset(cx, cy, m_hWnd);
	}
	// ================================================================================================
}


void CEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// ================================================================================================
	RECT rect = {};
	GetClientRect(&rect);
	Screen::GetInstance()->Initialize(m_hWnd, rect.right - rect.left, rect.bottom - rect.top);
	editor_main::Initialize();
	GraphicDevice::GetInstance()->SetBackgroundColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1));
	EditorResourceLoader::SimpleLoad();
	// ================================================================================================
}


void CEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);
	if (EditorManager::GetInstance())
		EditorManager::GetInstance()->SetFocus(true);
}


void CEditorView::OnKillFocus(CWnd* pNewWnd)
{
	CView::OnKillFocus(pNewWnd);
	if (EditorManager::GetInstance())
		EditorManager::GetInstance()->SetFocus(false);
}


void CEditorView::OnFileNew()
{
	EditorManager::GetInstance()->New();

	CMainFrame* frm = (CMainFrame*)AfxGetApp()->GetMainWnd();
	frm->SetWindowTextW(L"Untitled");
}


void CEditorView::OnFileOpen()
{
	CString str = _T("txt 파일(*.txt)|*.txt;|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		EditorManager::GetInstance()->Open(strPathName.GetString());

		CMainFrame* frm = (CMainFrame*)AfxGetApp()->GetMainWnd();
		frm->SetWindowTextW(strPathName);
	}
}


void CEditorView::OnFileSave()
{
	wstring currentPath = EditorManager::GetInstance()->GetCurrentPath();
	if (currentPath == L"")
	{
		OnFileSaveAs();

		CMainFrame* frm = (CMainFrame*)AfxGetApp()->GetMainWnd();
		frm->SetWindowTextW(currentPath.c_str());
	}
	else
	{
		EditorManager::GetInstance()->SaveAs(currentPath);

		CMainFrame* frm = (CMainFrame*)AfxGetApp()->GetMainWnd();
		frm->SetWindowTextW(currentPath.c_str());
	}
}


void CEditorView::OnFileSaveAs()
{
	CString str = _T("txt 파일(*.txt)|*.txt;|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(FALSE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		EditorManager::GetInstance()->SaveAs(strPathName.GetString());

		CMainFrame* frm = (CMainFrame*)AfxGetApp()->GetMainWnd();
		frm->SetWindowTextW(strPathName);
	}
}


void CEditorView::OnBackgroundfront()
{
	if (!m_dlgBackground.GetSafeHwnd())
		m_dlgBackground.Create(IDD_DlgBackground);
	m_dlgBackground.SetBackground(EditorManager::GetInstance()->GetFrontBackground());
	m_dlgBackground.ShowWindow(SW_SHOW);
}


void CEditorView::OnBackgroundmiddle()
{
	if (!m_dlgBackground.GetSafeHwnd())
		m_dlgBackground.Create(IDD_DlgBackground);
	m_dlgBackground.SetBackground(EditorManager::GetInstance()->GetMiddleBackground());
	m_dlgBackground.ShowWindow(SW_SHOW);
}


void CEditorView::OnBackgroundback()
{
	if (!m_dlgBackground.GetSafeHwnd())
		m_dlgBackground.Create(IDD_DlgBackground);
	m_dlgBackground.SetBackground(EditorManager::GetInstance()->GetBackBackground());
	m_dlgBackground.ShowWindow(SW_SHOW);
}


void CEditorView::OnCollider()
{
	if (!m_dlgCollider.GetSafeHwnd())
		m_dlgCollider.Create(IDD_DlgCollider);
	m_dlgCollider.ShowWindow(SW_SHOW);
}


void CEditorView::OnParallaxfront()
{
	if (!m_dlgParallax.GetSafeHwnd())
		m_dlgParallax.Create(IDD_DlgParallax);
	m_dlgParallax.SetParallax(EditorManager::GetInstance()->GetFrontParallax());
	m_dlgParallax.ShowWindow(SW_SHOW);
}


void CEditorView::OnParallaxmiddle()
{
	if (!m_dlgParallax.GetSafeHwnd())
		m_dlgParallax.Create(IDD_DlgParallax);
	m_dlgParallax.SetParallax(EditorManager::GetInstance()->GetMiddleParallax());
	m_dlgParallax.ShowWindow(SW_SHOW);
}


void CEditorView::OnParallaxback()
{
	if (!m_dlgParallax.GetSafeHwnd())
		m_dlgParallax.Create(IDD_DlgParallax);
	m_dlgParallax.SetParallax(EditorManager::GetInstance()->GetBackParallax());
	m_dlgParallax.ShowWindow(SW_SHOW);
}


void CEditorView::OnObject()
{
	if (!m_dlgObjectTool.GetSafeHwnd())
		m_dlgObjectTool.Create(IDD_DlgObjectTool);
	m_dlgObjectTool.ShowWindow(SW_SHOW);
}


void CEditorView::OnCameraborder()
{
	if (!m_dlgCameraBorderBox.GetSafeHwnd())
		m_dlgCameraBorderBox.Create(IDD_DlgCameraBorderBox);
	m_dlgCameraBorderBox.ShowWindow(SW_SHOW);
}


void CEditorView::OnTrigger()
{
	if (!m_dlgTrigger.GetSafeHwnd())
		m_dlgTrigger.Create(IDD_DlgTrigger);
	m_dlgTrigger.ShowWindow(SW_SHOW);
}
