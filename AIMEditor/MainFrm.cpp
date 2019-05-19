
// MainFrm.cpp : CMainFrame Ŭ������ ����
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

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �������� Ŭ���̾�Ʈ ������ �����ϴ� �並 ����ϴ�.
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.cx = 2280;
	cs.cy = 720;
	return TRUE;
}

// CMainFrame ����

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