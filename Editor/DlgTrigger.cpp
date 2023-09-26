// DlgTrigger.cpp: 구현 파일
//

#include "editor_stdafx.h"
#include "Editor.h"
#include "DlgTrigger.h"
#include "afxdialogex.h"

#include "TriggerBox.h"

// DlgTrigger 대화 상자

IMPLEMENT_DYNAMIC(DlgTrigger, CDialog)

DlgTrigger::DlgTrigger(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DlgTrigger, pParent)
{

}

DlgTrigger::~DlgTrigger()
{
}

void DlgTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, Trigget_Edit_Name, m_edit_name);
}


BEGIN_MESSAGE_MAP(DlgTrigger, CDialog)
	ON_BN_CLICKED(Trigger_Button_Create, &DlgTrigger::OnButtonCreate)
END_MESSAGE_MAP()


// DlgTrigger 메시지 처리기


void DlgTrigger::OnButtonCreate()
{
	WCHAR buffer[256];
	m_edit_name.GetWindowTextW(buffer, 256);

	if (buffer[0] == 0)
	{
		MessageBoxW(L"생성 불가: 빈 이름은 사용할 수 없습니다.");
		return;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (buffer[i] == 0) break;
		if (buffer[i] == L' ')
		{
			MessageBoxW(L"생성 불가: 이름에는 공백이 존재할 수 없습니다.");
			return;
		}
	}

	for (auto& trigger : TriggerBox::triggers)
	{
		if (trigger->m_name == buffer)
		{
			MessageBoxW(L"생성 불가: 중복된 이름을 가진 트리거가 존재합니다!");
			return;
		}
	}

	const Vec2& pos = CCamera2D::GetInstance()->GetPosition();
	auto obj = new GameObject(pos);
	auto trigger = obj->AddComponent<TriggerBox>();

	trigger->m_name = buffer;
}
