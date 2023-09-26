// DlgParallax.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgParallax.h"
#include "afxdialogex.h"

#include "Parallax.h"

namespace fs = std::filesystem;

// DlgParallax 대화 상자

IMPLEMENT_DYNAMIC(DlgParallax, CDialog)

void DlgParallax::SetParallax(Parallax* parallax)
{
	m_parallax = parallax;
}

DlgParallax::DlgParallax(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgParallax, pParent)
{

}

DlgParallax::~DlgParallax()
{
}

void DlgParallax::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, Parallax_Edit_Key, m_edit_key);
	DDX_Control(pDX, Parallax_Edit_Scale, m_edit_scale);
	DDX_Control(pDX, Parallax_Edit_BaseX, m_edit_base_x);
	DDX_Control(pDX, Parallax_Edit_BaseY, m_edit_base_y);
	DDX_Control(pDX, Parallax_Edit_ParallaxX, m_edit_parallax_x);
	DDX_Control(pDX, Parallax_Edit_ParallaxY, m_edit_parallax_y);
}


BEGIN_MESSAGE_MAP(DlgParallax, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(Parallax_Button_ChangeKey, &DlgParallax::OnButtonChangeKey)
	ON_EN_CHANGE(Parallax_Edit_Scale, &DlgParallax::OnChangeEditScale)
	ON_EN_CHANGE(Parallax_Edit_BaseX, &DlgParallax::OnChangeEditBaseX)
	ON_EN_CHANGE(Parallax_Edit_BaseY, &DlgParallax::OnChangeEditBaseY)
	ON_EN_CHANGE(Parallax_Edit_ParallaxX, &DlgParallax::OnChangeEditParallaxX)
	ON_EN_CHANGE(Parallax_Edit_ParallaxY, &DlgParallax::OnChangeEditParallaxY)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// DlgParallax 메시지 처리기


void DlgParallax::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (!m_parallax) return;

	m_edit_key.SetWindowTextW(m_parallax->GetKey().c_str());
	m_edit_scale.SetWindowTextW(std::to_wstring(m_parallax->GetScale()).c_str());
	m_edit_base_x.SetWindowTextW(std::to_wstring(m_parallax->GetBase().x).c_str());
	m_edit_base_y.SetWindowTextW(std::to_wstring(m_parallax->GetBase().y).c_str());
	m_edit_parallax_x.SetWindowTextW(std::to_wstring(m_parallax->GetParallaxScale().x).c_str());
	m_edit_parallax_y.SetWindowTextW(std::to_wstring(m_parallax->GetParallaxScale().y).c_str());
}


void DlgParallax::OnButtonChangeKey()
{
	WCHAR buffer[256];
	m_edit_key.GetWindowTextW(buffer, 256);

	if (!m_parallax) return;
	m_parallax->SetSprite(buffer);
}


void DlgParallax::OnChangeEditScale()
{
	WCHAR buffer[256], * cursor;
	m_edit_scale.GetWindowTextW(buffer, 256);
	float scale = wcstof(buffer, &cursor);

	if (!m_parallax) return;
	m_parallax->SetScale(scale);
}


void DlgParallax::OnChangeEditBaseX()
{
	WCHAR buffer[256], * cursor;
	m_edit_base_x.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	if (!m_parallax) return;
	Vec2 vec = m_parallax->GetBase();
	vec.x = value;

	m_parallax->SetBase(vec);
}


void DlgParallax::OnChangeEditBaseY()
{
	WCHAR buffer[256], * cursor;
	m_edit_base_y.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	if (!m_parallax) return;
	Vec2 vec = m_parallax->GetBase();
	vec.y = value;

	m_parallax->SetBase(vec);
}


void DlgParallax::OnChangeEditParallaxX()
{
	WCHAR buffer[256], * cursor;
	m_edit_parallax_x.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	if (!m_parallax) return;
	Vec2 vec = m_parallax->GetParallaxScale();
	vec.x = value;

	m_parallax->SetParallaxScale(vec);
}


void DlgParallax::OnChangeEditParallaxY()
{
	WCHAR buffer[256], * cursor;
	m_edit_parallax_y.GetWindowTextW(buffer, 256);
	float value = wcstof(buffer, &cursor);

	if (!m_parallax) return;
	Vec2 vec = m_parallax->GetParallaxScale();
	vec.y = value;

	m_parallax->SetParallaxScale(vec);
}



void DlgParallax::OnDropFiles(HDROP hDropInfo)
{
	WCHAR buffer[MAX_PATH];
	DWORD fileCount = DragQueryFile(hDropInfo, 0xffffffff, buffer, MAX_PATH);

	if (fileCount > 0)
	{
		DragQueryFile(hDropInfo, 0, buffer, MAX_PATH);
		std::replace(buffer, buffer + MAX_PATH, L'\\', L'/');

		fs::path path = buffer;
		fs::path rel_path = fs::relative(path, fs::current_path());

		wstring w_rel_path = rel_path;
		std::replace(w_rel_path.begin(), w_rel_path.end(), L'\\', L'/');

		m_edit_key.SetWindowTextW(w_rel_path.c_str());
	}

	CDialog::OnDropFiles(hDropInfo);
}
