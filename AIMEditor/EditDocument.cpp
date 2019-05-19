// EditDocument.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AIMEditor.h"
#include "EditDocument.h"


// EditDocument

IMPLEMENT_DYNCREATE(EditDocument, CDocument)

EditDocument::EditDocument()
{
}

BOOL EditDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

EditDocument::~EditDocument()
{
}


BEGIN_MESSAGE_MAP(EditDocument, CDocument)
END_MESSAGE_MAP()


// EditDocument 진단입니다.

#ifdef _DEBUG
void EditDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void EditDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// EditDocument serialization입니다.

void EditDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}
#endif


// EditDocument 명령입니다.
