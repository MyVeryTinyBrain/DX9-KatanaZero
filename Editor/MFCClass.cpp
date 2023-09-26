// MFCClass.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "MFCClass.h"


// MFCClass

IMPLEMENT_DYNCREATE(MFCClass, CFormView)

MFCClass::MFCClass()
	: CFormView()
{

}

MFCClass::~MFCClass()
{
}

void MFCClass::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(MFCClass, CFormView)
END_MESSAGE_MAP()


// MFCClass 진단

#ifdef _DEBUG
void MFCClass::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MFCClass::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MFCClass 메시지 처리기
