#pragma once

class Background;

// DlgBackground 대화 상자

class DlgBackground : public CDialog
{
	PUBLIC void SetBackground(Background* bg);

	PRIVATE Background* m_bg;

	DECLARE_DYNAMIC(DlgBackground)

public:
	DlgBackground(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgBackground();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgBackground };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_edit_key;
	CEdit m_edit_scale;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonChangeKey();
	afx_msg void OnEditChangeScale();
	afx_msg void OnChangeEditRed();
	afx_msg void OnChangeEditGreen();
	afx_msg void OnChangeEditBlue();
public:
	CEdit m_edit_red;
	CEdit m_edit_green;
	CEdit m_edit_blue;
	CEdit m_edit_adjustX;
	CEdit m_edit_adjustY;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnChangeEditAdjustX();
	afx_msg void OnChangeEditAdjustY();
};
