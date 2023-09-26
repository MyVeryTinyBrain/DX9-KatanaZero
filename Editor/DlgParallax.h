#pragma once

class Parallax;

// DlgParallax 대화 상자

class DlgParallax : public CDialog
{
	PUBLIC void SetParallax(Parallax* parallax);

	PRIVATE Parallax* m_parallax;

	DECLARE_DYNAMIC(DlgParallax)

public:
	DlgParallax(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgParallax();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgParallax };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit m_edit_key;
	CEdit m_edit_scale;
	CEdit m_edit_base_x;
	CEdit m_edit_base_y;
	CEdit m_edit_parallax_x;
	CEdit m_edit_parallax_y;
	afx_msg void OnButtonChangeKey();
	afx_msg void OnChangeEditScale();
	afx_msg void OnChangeEditBaseX();
	afx_msg void OnChangeEditBaseY();
	afx_msg void OnChangeEditParallaxX();
	afx_msg void OnChangeEditParallaxY();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
