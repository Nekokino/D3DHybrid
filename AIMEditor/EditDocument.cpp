// EditDocument.cpp : ���� �����Դϴ�.
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


// EditDocument �����Դϴ�.

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
// EditDocument serialization�Դϴ�.

void EditDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}
#endif


// EditDocument ����Դϴ�.
