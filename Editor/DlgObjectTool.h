#pragma once


// DlgObjectTool 대화 상자

class DlgObjectTool : public CDialog
{
	PRIVATE void DrawTexture(const wstring& key);
	PRIVATE void FillListContents(const wstring& directory);
	PRIVATE void SelectObject(const wstring& path);

	PRIVATE std::filesystem::path m_objectRoot; // 상대 경로(../EditorObject)
	PRIVATE std::filesystem::path m_current;
	PRIVATE bool m_hasSelValue;
	PRIVATE Json::Value m_selValue;
	PRIVATE SelectParameters m_selParam;

	DECLARE_DYNAMIC(DlgObjectTool)

public:
	DlgObjectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DlgObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgObjectTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonCreate();
	CStatic m_picture;
	CListBox m_list;
	afx_msg void OnListSelChange();
	afx_msg void OnListDBClick();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CButton m_radio_left;
	CButton m_radio_right;
};
