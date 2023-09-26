#pragma once


// DlgCollider 대화 상자

class DlgCollider : public CDialog
{
	DECLARE_DYNAMIC(DlgCollider)

public:
	DlgCollider(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgCollider();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgCollider };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnButtonBox();
	afx_msg void OnButtonLeftTriangle();
	afx_msg void OnButtonRightTriangle();
	afx_msg void OnButtonLine();
	CButton m_check_wall;
};
