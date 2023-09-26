// DlgCameraBorderBox.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgCameraBorderBox.h"
#include "afxdialogex.h"

#include "EditorManager.h"
#include "CameraBorderBox.h"

// DlgCameraBorderBox 대화 상자

IMPLEMENT_DYNAMIC(DlgCameraBorderBox, CDialog)

DlgCameraBorderBox::DlgCameraBorderBox(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgCameraBorderBox, pParent)
{

}

DlgCameraBorderBox::~DlgCameraBorderBox()
{
}

void DlgCameraBorderBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, CameraBorder_Check_Show, m_check_show);
}


BEGIN_MESSAGE_MAP(DlgCameraBorderBox, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(CameraBorder_Button_Reset, &DlgCameraBorderBox::OnButtonReset)
	ON_BN_CLICKED(CameraBorder_Button_Fit, &DlgCameraBorderBox::OnButtonFit)
	ON_BN_CLICKED(CameraBorder_Check_Show, &DlgCameraBorderBox::OnCheckShow)
END_MESSAGE_MAP()


// DlgCameraBorderBox 메시지 처리기


void DlgCameraBorderBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	auto camBorder = EditorManager::GetInstance()->GetCameraBorderBox();
	if (!camBorder) return;

	m_check_show.SetCheck(camBorder->m_show);
}


void DlgCameraBorderBox::OnButtonReset()
{
	auto camBorder = EditorManager::GetInstance()->GetCameraBorderBox();
	if (!camBorder) return;

	camBorder->Reset();
	m_check_show.SetCheck(camBorder->m_show);
}


void DlgCameraBorderBox::OnButtonFit()
{
	auto camBorder = EditorManager::GetInstance()->GetCameraBorderBox();
	if (!camBorder) return;

	camBorder->FitToBackground();
	m_check_show.SetCheck(camBorder->m_show);
}


void DlgCameraBorderBox::OnCheckShow()
{
	auto camBorder = EditorManager::GetInstance()->GetCameraBorderBox();
	if (!camBorder) return;

	camBorder->m_show = m_check_show.GetCheck();
}
