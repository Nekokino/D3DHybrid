
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "AIMEditor.h"

#include "MainFrm.h"
#include "EditView.h"
#include "EditFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 프레임의 클라이언트 영역을 차지하는 뷰를 만듭니다.
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.cx = 2280;
	cs.cy = 720;
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT _lpcs, CCreateContext* _Context)
{
	SplitWindow.CreateStatic(this, 1, 2);

	SplitWindow.CreateView(0, 0, RUNTIME_CLASS(EditView), CSize(1280, 720), _Context);
	SplitWindow.CreateView(0, 1, RUNTIME_CLASS(EditFormView), CSize(1000, 720), _Context);

	View = (EditView*)SplitWindow.GetPane(0, 0);
	FormView = (EditFormView*)SplitWindow.GetPane(0, 1);

	return TRUE;
}