// DlgBackground.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgBackground.h"
#include "afxdialogex.h"

#include "EditorManager.h"
#include "Background.h"

namespace fs = std::filesystem;

// DlgBackground 대화 상자

IMPLEMENT_DYNAMIC(DlgBackground, CDialog)

void DlgBackground::SetBackground(Background* bg)
{
	m_bg = bg;
}

DlgBackground::DlgBackground(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgBackground, pParent)
{

}

DlgBackground::~DlgBackground()
{
}

void DlgBackground::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, Background_Edit_Key, m_edit_key);
	DDX_Control(pDX, Background_Edit_Scale, m_edit_scale);
	DDX_Control(pDX, Background_Edit_Red, m_edit_red);
	DDX_Control(pDX, Background_Edit_Green, m_edit_green);
	DDX_Control(pDX, Background_Edit_Blue, m_edit_blue);
	DDX_Control(pDX, Background_Edit_AdjustX, m_edit_adjustX);
	DDX_Control(pDX, Background_Edit_AdjustY, m_edit_adjustY);
}


BEGIN_MESSAGE_MAP(DlgBackground, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(Background_Button_ChangeKey, &DlgBackground::OnButtonChangeKey)
	ON_EN_CHANGE(Background_Edit_Scale, &DlgBackground::OnEditChangeScale)
	ON_EN_CHANGE(Background_Edit_Red, &DlgBackground::OnChangeEditRed)
	ON_EN_CHANGE(Background_Edit_Green, &DlgBackground::OnChangeEditGreen)
	ON_EN_CHANGE(Background_Edit_Blue, &DlgBackground::OnChangeEditBlue)
	ON_EN_CHANGE(Background_Edit_AdjustX, &DlgBackground::OnChangeEditAdjustX)
	ON_EN_CHANGE(Background_Edit_AdjustY, &DlgBackground::OnChangeEditAdjustY)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// DlgBackground 메시지 처리기


void DlgBackground::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Background* bg = m_bg;
	if (!bg) return;

	m_edit_key.SetWindowTextW(bg->GetKey().c_str());
	m_edit_scale.SetWindowTextW(std::to_wstring(bg->GetScale()).c_str());
	m_edit_red.SetWindowTextW(std::to_wstring(bg->GetEmptyColor().r * 255.0f).c_str());
	m_edit_green.SetWindowTextW(std::to_wstring(bg->GetEmptyColor().g * 255.0f).c_str());
	m_edit_blue.SetWindowTextW(std::to_wstring(bg->GetEmptyColor().b * 255.0f).c_str());
	m_edit_adjustX.SetWindowTextW(std::to_wstring(bg->GetTransform()->GetPosition().x).c_str());
	m_edit_adjustY.SetWindowTextW(std::to_wstring(bg->GetTransform()->GetPosition().y).c_str());
}


void DlgBackground::OnButtonChangeKey()
{
	WCHAR buffer[256];
	m_edit_key.GetWindowTextW(buffer, 256);

	Background* bg = m_bg;
	if (!bg) return;
	bg->SetSprite(buffer);
}


void DlgBackground::OnEditChangeScale()
{
	WCHAR buffer[256], * cursor;
	m_edit_scale.GetWindowTextW(buffer, 256);
	float scale = wcstof(buffer, &cursor);

	Background* bg = m_bg;
	if (!bg) return;
	bg->SetScale(scale);
}


void DlgBackground::OnChangeEditRed()
{
	WCHAR buffer[256], * cursor;
	m_edit_red.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	Background* bg = m_bg;
	if (!bg) return;

	Color color = bg->GetEmptyColor();
	color.r = value / 255.0f;
	bg->SetEmptyColor(color);
}


void DlgBackground::OnChangeEditGreen()
{
	WCHAR buffer[256], * cursor;
	m_edit_green.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	Background* bg = m_bg;
	if (!bg) return;
	
	Color color = bg->GetEmptyColor();
	color.g = value / 255.0f;
	bg->SetEmptyColor(color);
}


void DlgBackground::OnChangeEditBlue()
{
	WCHAR buffer[256], * cursor;
	m_edit_blue.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	Background* bg = m_bg;
	if (!bg) return;

	Color color = bg->GetEmptyColor();
	color.b = value / 255.0f;
	bg->SetEmptyColor(color);
}


void DlgBackground::OnChangeEditAdjustX()
{
	Background* bg = m_bg;
	if (!bg) return;

	WCHAR buffer[256], * cursor;
	m_edit_adjustX.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	Vec2 vec = bg->GetTransform()->GetPosition();
	vec.x = value;

	bg->GetTransform()->SetPosition(vec);
}


void DlgBackground::OnChangeEditAdjustY()
{
	Background* bg = m_bg;
	if (!bg) return;

	WCHAR buffer[256], * cursor;
	m_edit_adjustY.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	Vec2 vec = bg->GetTransform()->GetPosition();
	vec.y = value;

	bg->GetTransform()->SetPosition(vec);
}


void DlgBackground::OnDropFiles(HDROP hDropInfo)
{
	WCHAR buffer[MAX_PATH];
	DWORD fileCount = DragQueryFile(hDropInfo, 0xffffffff, buffer, MAX_PATH);

	if (fileCount > 0)
	{
		DragQueryFile(hDropInfo, 0, buffer, MAX_PATH);

		fs::path path = buffer;
		fs::path rel_path = fs::relative(path, fs::current_path());

		wstring w_rel_path = rel_path;
		std::replace(w_rel_path.begin(), w_rel_path.end(), L'\\', L'/');

		m_edit_key.SetWindowTextW(w_rel_path.c_str());
	}

	CDialog::OnDropFiles(hDropInfo);
}
