#pragma once

// EditView ���Դϴ�.

#include "EditDocument.h"

class EditView : public CView
{
	DECLARE_DYNCREATE(EditView)

protected:
	EditView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~EditView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


