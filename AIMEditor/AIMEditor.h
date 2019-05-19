
// AIMEditor.h : AIMEditor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


#ifdef _DEBUG
	#ifdef _M_IX86
		#pragma comment(lib, "AIMEngineD32")
	#else
		#pragma comment(lib, "AIMEngineD64")
	#endif
#else
	#ifdef _M_IX86
		#pragma comment(lib, "AIMEngineR32")
	#else
		#pragma comment(lib, "AIMEngineR64")
	#endif
#endif

// CAIMEditorApp:
// �� Ŭ������ ������ ���ؼ��� AIMEditor.cpp�� �����Ͻʽÿ�.
//

class CAIMEditorApp : public CWinApp
{
public:
	CAIMEditorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CAIMEditorApp theApp;
