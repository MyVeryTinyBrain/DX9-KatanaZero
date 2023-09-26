#pragma once


// DlgCameraBorderBox 대화 상자

class DlgCameraBorderBox : public CDialog
{
	DECLARE_DYNAMIC(DlgCameraBorderBox)

public:
	DlgCameraBorderBox(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgCameraBorderBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgCameraBorderBox };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_check_show;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonFit();
	afx_msg void OnCheckShow();
	afx_msg void OnButtonReset();
};
