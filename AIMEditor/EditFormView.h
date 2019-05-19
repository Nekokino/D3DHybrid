#pragma once

#include "afxwin.h"

#include <AIMObject.h>
#include <AIMAnimation.h>
#include <AIMRenderer.h>

// EditFormView �� ���Դϴ�.

class EditFormView : public CFormView
{
	DECLARE_DYNCREATE(EditFormView)

protected:
	EditFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~EditFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.



private:
	Ezptr<AIMObject> Obj;
	Ezptr<AIMAnimation> Animation;
	Ezptr<AIMRenderer> Renderer;
	std::vector<Ezptr<BoneKeyFrame>> KeyFrameVec;

	DECLARE_MESSAGE_MAP()
public:
	CString Name;
	CString ClipName;
	UINT StartFrame;
	UINT EndFrame;
	float PlayTime;
	afx_msg void OnBnClickedAddclip();
	afx_msg void OnBnClickedModifyclip();
	afx_msg void OnBnClickedDeleteclip();
	afx_msg void OnBnClickedMeshload();
	afx_msg void OnBnClickedMeshsave();
	afx_msg void OnBnClickedFbxload();
	afx_msg void OnBnClickedClipload();
	afx_msg void OnBnClickedClipsave();
	CListBox ClipList;
	CComboBox OptionCombo;
	afx_msg void OnLbnSelchangeCliplist();
	afx_msg void OnBnClickedFramecopy();
	BOOL bFrameCopy;
};


