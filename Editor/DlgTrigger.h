#pragma once


// DlgTrigger 대화 상자

class DlgTrigger : public CDialog
{
	DECLARE_DYNAMIC(DlgTrigger)

public:
	DlgTrigger(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgTrigger();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgTrigger };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_name;
	afx_msg void OnButtonCreate();
};
