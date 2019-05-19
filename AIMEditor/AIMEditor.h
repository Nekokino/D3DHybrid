
// AIMEditor.h : AIMEditor 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


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
// 이 클래스의 구현에 대해서는 AIMEditor.cpp을 참조하십시오.
//

class CAIMEditorApp : public CWinApp
{
public:
	CAIMEditorApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CAIMEditorApp theApp;
