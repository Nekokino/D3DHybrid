// EditView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AIMEditor.h"
#include "EditView.h"
#include <Core.h>


// EditView

IMPLEMENT_DYNCREATE(EditView, CView)

EditView::EditView()
{

}

EditView::~EditView()
{
	
}

BEGIN_MESSAGE_MAP(EditView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// EditView 그리기입니다.

void EditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// EditView 진단입니다.

#ifdef _DEBUG
void EditView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void EditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// EditView 메시지 처리기입니다.



int EditView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	Core::Inst()->Init(AfxGetInstanceHandle(), m_hWnd, 1280, 720);

	return 0;
}


void EditView::OnDestroy()
{
	CView::OnDestroy();

	Core::Inst()->DestroyInst();
}
