// DlgObjectTool.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgObjectTool.h"
#include "afxdialogex.h"

#include "EditorObject.h"
#include <fstream>

namespace fs = std::filesystem;

// DlgObjectTool 대화 상자

IMPLEMENT_DYNAMIC(DlgObjectTool, CDialog)

void DlgObjectTool::DrawTexture(const wstring& key)
{
	Texture* tex = TextureManager::GetTexture(key);
	
	RECT pictureRect;
	m_picture.GetClientRect(&pictureRect);
	float picWidth = float(pictureRect.right - pictureRect.left);
	float picHeight = float(pictureRect.bottom - pictureRect.top);
	float camWidth = CCamera2D::GetInstance()->GetWidth();
	float camHeight = CCamera2D::GetInstance()->GetHeight();
	Vec2 camPos = CCamera2D::GetInstance()->GetPosition();

	// 카메라가 PictureControll을 딱 채우도록 변경합니다.
	CCamera2D::GetInstance()->SetPosition(Vec2::zero());
	CCamera2D::GetInstance()->SetWidth(picWidth);
	CCamera2D::GetInstance()->SetHeight(picHeight);

	// PictureControll에 딱 맞게 늘리는 스케일링을 구합니다.
	// 하지만 비율 유지를 위해서 작게 늘어나는 스케일링 값을 사용합니다.
	Vec2 imageSize = Vec2(float(tex->GetImageInfo().Width), float(tex->GetImageInfo().Height));
	Vec2 scaling = Vec2(picWidth / imageSize.x, picHeight / imageSize.y);
	if (scaling.x < scaling.y) scaling.y = scaling.x;
	if (scaling.x > scaling.y) scaling.x = scaling.y;

	Vec3 scale = Vec3(scaling.x, scaling.y, 1);
	Vec3 angle = Vec3::zero();
	Vec3 pos = Vec3::zero();
	Mat4 m = Mat4::SRT(scale, angle, pos) * CCamera2D::GetInstance()->GetWorldToCameraMatrix() * Screen::GetCameraToScreenMatrix();

	GraphicDevice::GetInstance()->BeginRender();
	GraphicDevice::GetSpriteDevice()->DrawSingleTextrue(tex, m, Color(1, 1, 1, 1));
	GraphicDevice::GetInstance()->EndRender(m_picture.m_hWnd);

	// 카메라의 스케일을 다시 되돌립니다.
	CCamera2D::GetInstance()->SetPosition(camPos);
	CCamera2D::GetInstance()->SetWidth(camWidth);
	CCamera2D::GetInstance()->SetHeight(camHeight);
}

void DlgObjectTool::FillListContents(const wstring& directory)
{
	m_list.ResetContent();

	if (directory != m_objectRoot)
	{
		m_list.AddString(L"..");
	}

	for (const fs::directory_entry& entry : fs::directory_iterator(directory))
	{
		const fs::path& path = entry.path();
		if (fs::is_directory(path))
		{
			fs::path relPath = fs::relative(path, fs::current_path());
			m_list.AddString(relPath.c_str());
		}
		else if (path.extension() == L".txt")
		{
			fs::path name = path.filename();
			name.replace_extension(L"");
			m_list.AddString(name.c_str());
		}
	}
}

void DlgObjectTool::SelectObject(const wstring& path)
{
	Json::Value root;
	std::ifstream ifs;
	ifs.open(path);

	if (!ifs.is_open()) return;

	Json::CharReaderBuilder builder;
	JSONCPP_STRING errs;
	if (parseFromStream(builder, ifs, &root, &errs))
	{
		m_selValue = root;
		m_hasSelValue = true;

		string s_sprite = root["sprite"].asString() + ".png";
		USES_CONVERSION;
		wstring texKey(A2W(s_sprite.c_str()));
		DrawTexture(texKey);
	}
	else
	{
		ifs.close();
	}

	ifs.close();
}

DlgObjectTool::DlgObjectTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgObjectTool, pParent)
{
	fs::path current = fs::current_path();
	fs::path root = current.parent_path();
	m_objectRoot = root / L"EditorObject";
	m_objectRoot = fs::relative(m_objectRoot, current);
	m_current = m_objectRoot;
}

DlgObjectTool::~DlgObjectTool()
{
}

void DlgObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ObjectTool_Picture, m_picture);
	DDX_Control(pDX, ObjectTool_List, m_list);
	DDX_Control(pDX, Object_Radio_Left, m_radio_left);
	DDX_Control(pDX, Object_Radio_Right, m_radio_right);
}


BEGIN_MESSAGE_MAP(DlgObjectTool, CDialog)
	ON_BN_CLICKED(ObjectTool_Button_Create, &DlgObjectTool::OnButtonCreate)
	ON_LBN_SELCHANGE(ObjectTool_List, &DlgObjectTool::OnListSelChange)
	ON_LBN_DBLCLK(ObjectTool_List, &DlgObjectTool::OnListDBClick)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// DlgObjectTool 메시지 처리기


void DlgObjectTool::OnButtonCreate()
{
	if (m_hasSelValue)
	{
		m_selParam.horizontalDirection = float(m_radio_left.GetCheck() ? -1 : +1);
		EditorObject::Create(m_selValue, m_selParam);
	}
}


void DlgObjectTool::OnListSelChange()
{
	int index = m_list.GetCurSel();
	CString cstring;
	m_list.GetText(index, cstring);
	wstring select = cstring;

	if (select == L"..")
	{
		return;
	}
	else if (fs::is_directory(select))
	{
		return;
	}
	else
	{
		fs::path dir = m_current / select;
		wstring path = dir.wstring() + L".txt";
		SelectObject(path);
	}
}


void DlgObjectTool::OnListDBClick()
{
	int index = m_list.GetCurSel();
	CString cstring;
	m_list.GetText(index, cstring);
	wstring select = cstring;

	if (select == L"..")
	{
		m_current = m_current.parent_path();
		FillListContents(m_current);
	}
	else if (fs::is_directory(select))
	{
		m_current = select;
		FillListContents(m_current);
	}
	else
	{
		fs::path dir = m_current / select;
		wstring path = dir.wstring() + L".txt";
		SelectObject(path);
	}
}


void DlgObjectTool::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	FillListContents(m_current);
	m_hasSelValue = false;

	m_radio_left.SetCheck(false);
	m_radio_right.SetCheck(true);
}
