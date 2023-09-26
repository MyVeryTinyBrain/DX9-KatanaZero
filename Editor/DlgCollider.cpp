// DlgCollider.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgCollider.h"
#include "afxdialogex.h"

#include "EditorBoxCollider.h"
#include "EditorRightTriangleCollider.h"
#include "EditorLeftTriangleCollider.h"
#include "EditorLineCollider.h"


// DlgCollider 대화 상자

IMPLEMENT_DYNAMIC(DlgCollider, CDialog)

DlgCollider::DlgCollider(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgCollider, pParent)
{

}

DlgCollider::~DlgCollider()
{
}

void DlgCollider::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, Collider_Check_Wall, m_check_wall);
}


BEGIN_MESSAGE_MAP(DlgCollider, CDialog)
	ON_BN_CLICKED(Collider_Button_box, &DlgCollider::OnButtonBox)
	ON_BN_CLICKED(Collider_Button_LeftTriangle, &DlgCollider::OnButtonLeftTriangle)
	ON_BN_CLICKED(Collider_Button_RightTriangle, &DlgCollider::OnButtonRightTriangle)
	ON_BN_CLICKED(Collider_Button_Line, &DlgCollider::OnButtonLine)
END_MESSAGE_MAP()


// DlgCollider 메시지 처리기


void DlgCollider::OnButtonBox()
{
	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();
	auto obj = new GameObject(pos);
	auto col = obj->AddComponent<EditorBoxCollider>();
	if (m_check_wall.GetCheck()) col->type = COLLIDER_WALL;
}


void DlgCollider::OnButtonLeftTriangle()
{
	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();
	auto obj = new GameObject(pos);
	auto col = obj->AddComponent<EditorLeftTriangleCollider>();
	if (m_check_wall.GetCheck()) col->type = COLLIDER_WALL;
}


void DlgCollider::OnButtonRightTriangle()
{
	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();
	auto obj = new GameObject(pos);
	auto col = obj->AddComponent<EditorRightTriangleCollider>();
	if (m_check_wall.GetCheck()) col->type = COLLIDER_WALL;
}


void DlgCollider::OnButtonLine()
{
	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();
	auto obj = new GameObject(pos);
	auto col = obj->AddComponent<EditorLineCollider>();
	if (m_check_wall.GetCheck()) col->type = COLLIDER_WALL;
}
