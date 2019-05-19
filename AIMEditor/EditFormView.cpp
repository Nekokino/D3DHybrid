// EditFormView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AIMEditor.h"
#include "EditFormView.h"

#include <AIMScene.h>
#include <SceneManager.h>
#include <AIMLayer.h>
#include <ResourceManager.h>
#include <AIMTransform.h>


// EditFormView

IMPLEMENT_DYNCREATE(EditFormView, CFormView)

EditFormView::EditFormView()
	: CFormView(IDD_EDITFORMVIEW)
	, Name(_T(""))
	, ClipName(_T(""))
	, StartFrame(0)
	, EndFrame(0)
	, PlayTime(0)
	, bFrameCopy(FALSE)
{

}

EditFormView::~EditFormView()
{
	KeyFrameVec.clear();
}

void EditFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, Name);
	DDX_Text(pDX, IDC_CLIPNAME, ClipName);
	DDX_Text(pDX, IDC_STARTFRAME, StartFrame);
	DDX_Text(pDX, IDC_ENDFRAME, EndFrame);
	DDX_Text(pDX, IDC_PLAYTIME, PlayTime);
	DDX_Control(pDX, IDC_CLIPLIST, ClipList);
	DDX_Control(pDX, IDC_OPTIONCOMBO, OptionCombo);
	DDX_Check(pDX, IDC_FRAMECOPY, bFrameCopy);
}

BEGIN_MESSAGE_MAP(EditFormView, CFormView)
	ON_BN_CLICKED(IDC_ADDCLIP, &EditFormView::OnBnClickedAddclip)
	ON_BN_CLICKED(IDC_MODIFYCLIP, &EditFormView::OnBnClickedModifyclip)
	ON_BN_CLICKED(IDC_DELETECLIP, &EditFormView::OnBnClickedDeleteclip)
	ON_BN_CLICKED(IDC_MESHLOAD, &EditFormView::OnBnClickedMeshload)
	ON_BN_CLICKED(IDC_MESHSAVE, &EditFormView::OnBnClickedMeshsave)
	ON_BN_CLICKED(IDC_FBXLOAD, &EditFormView::OnBnClickedFbxload)
	ON_BN_CLICKED(IDC_CLIPLOAD, &EditFormView::OnBnClickedClipload)
	ON_BN_CLICKED(IDC_CLIPSAVE, &EditFormView::OnBnClickedClipsave)
	ON_LBN_SELCHANGE(IDC_CLIPLIST, &EditFormView::OnLbnSelchangeCliplist)
	ON_BN_CLICKED(IDC_FRAMECOPY, &EditFormView::OnBnClickedFramecopy)
END_MESSAGE_MAP()


// EditFormView 진단입니다.

#ifdef _DEBUG
void EditFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void EditFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// EditFormView 메시지 처리기입니다.


void EditFormView::OnBnClickedAddclip()
{
	if (Animation == nullptr)
	{
		return;
	}

	UpdateData(TRUE);

	char TmpName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, ClipName.GetString(), -1, TmpName, ClipName.GetLength(), 0, 0);

	if (KeyFrameVec.empty() == false)
	{
		Animation->AddClip(TmpName, (AnimationOption)OptionCombo.GetCurSel(), StartFrame, EndFrame, PlayTime, KeyFrameVec);

		ClipList.AddString(ClipName);
	}
}


void EditFormView::OnBnClickedModifyclip()
{
	if (Animation == nullptr)
	{
		return;
	}

	int ListIndex = ClipList.GetCurSel();

	if (ListIndex == -1)
	{
		return;
	}

	CString ListName;
	ClipList.GetText(ListIndex, ListName);

	char OriginName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, ListName.GetString(), -1, OriginName, ListName.GetLength(), 0, 0);

	UpdateData(TRUE);

	char TmpName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, ClipName.GetString(), -1, TmpName, ClipName.GetLength(), 0, 0);

	ClipList.DeleteString(ListIndex);
	ClipList.AddString(ClipName);

	if (KeyFrameVec.empty() == true)
	{
		Animation->ChangeClipName(OriginName, TmpName);
	}

	else
	{
		Animation->ModifyClip(OriginName, TmpName, (AnimationOption)OptionCombo.GetCurSel(), StartFrame, EndFrame, PlayTime, KeyFrameVec);
	}
}


void EditFormView::OnBnClickedDeleteclip()
{
	if (Animation == nullptr)
	{
		return;
	}

	else if (ClipList.GetCount() == 1)
	{
		return;
	}

	int ListIndex = ClipList.GetCurSel();

	if (ListIndex == -1)
	{
		return;
	}

	CString ListName;

	ClipList.GetText(ListIndex, ListName);

	char OriginName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, ListName.GetString(), -1, OriginName, ListName.GetLength(), 0, 0);

	char TmpName[256] = {};

	ClipList.DeleteString(ListIndex);

	Animation->DeleteClip(OriginName);

	Ezptr<AnimationClip> Clip = Animation->GetCurrentClip();

	ClipName = CA2CT(Clip->Name.c_str());
	StartFrame = Clip->StartFrame;
	EndFrame = Clip->EndFrame;
	PlayTime = Clip->PlayTime;
	OptionCombo.SetCurSel(Clip->Option);

	UpdateData(FALSE);

}


void EditFormView::OnBnClickedMeshload()
{
	const TCHAR* Filter = TEXT("Mesh File(*.amsh) |*.amsh| 모든파일 (*.*) |*.*||");

	CFileDialog dlg(TRUE, TEXT(".amsh"), nullptr, OFN_HIDEREADONLY, Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString Path = dlg.GetPathName();

		if (Obj == nullptr)
		{
			Ezptr<AIMScene> Scene = SceneManager::GetScene();
			Ezptr<AIMLayer> Layer = Scene->FindLayer("Default");

			Obj = AIMObject::CreateObject("Obj", Layer);

			Ezptr<AIMTransform> Transform = Obj->GetTransform();

			Transform->SetWorldScale(0.05f, 0.05f, 0.05f);
		}

		if (Renderer == nullptr)
		{
			Renderer = Obj->AddComponent<AIMRenderer>("Renderer");

			Renderer->SetMeshFromFullPath("EditMesh", Path.GetString());
		}
		else
		{
			ResourceManager::DeleteMesh("EditMesh");
			Renderer->SetMeshFromFullPath("EditMesh", Path.GetString());
		}

		Name = TEXT("EditMesh");

		UpdateData(FALSE);
	}
}


void EditFormView::OnBnClickedMeshsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditFormView::OnBnClickedFbxload()
{
	const TCHAR* Filter = TEXT("Fbx File(*.fbx) |*.fbx| 모든파일 (*.*) |*.*||");

	CFileDialog dlg(TRUE, TEXT(".fbx"), nullptr, OFN_HIDEREADONLY, Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString Path = dlg.GetPathName();

		if (Obj == nullptr)
		{
			Ezptr<AIMScene> Scene = SceneManager::GetScene();
			Ezptr<AIMLayer> Layer = Scene->FindLayer("Default");

			Obj = AIMObject::CreateObject("Obj", Layer);

			Ezptr<AIMTransform> Transform = Obj->GetTransform();

			Transform->SetWorldScale(0.05f, 0.05f, 0.05f);
		}

		if (Renderer == nullptr)
		{
			Renderer = Obj->AddComponent<AIMRenderer>("Renderer");

			Renderer->SetMeshFromFullPath("EditMesh", Path.GetString());
		}
		else
		{
			ResourceManager::DeleteMesh("EditMesh");
			Renderer->SetMeshFromFullPath("EditMesh", Path.GetString());
		}

		Animation = Obj->FindComponent<AIMAnimation>(CT_ANIMATION);

		const std::list<std::string>* Clips = Animation->GetAddClipList();

		std::list<std::string>::const_iterator StartIter = Clips->begin();
		std::list<std::string>::const_iterator EndIter = Clips->end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			CString ClipName = CA2CT(StartIter->c_str());

			ClipList.AddString(ClipName);
		}

		ClipList.SetCurSel(0);

		Ezptr<AnimationClip> Clip = Animation->GetCurrentClip();

		ClipName = CA2CT(Clip->Name.c_str());
		StartFrame = Clip->StartFrame;
		EndFrame = Clip->EndFrame;
		PlayTime = Clip->PlayTime;
		OptionCombo.SetCurSel(Clip->Option);

		UpdateData(FALSE);
	}
}


void EditFormView::OnBnClickedClipload()
{
	const TCHAR* Filter = TEXT("Animation File(*.anm)|*.anm|모든 파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(".anm"), nullptr, OFN_HIDEREADONLY, Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString Path = dlg.GetPathName();

		if (Obj == nullptr)
		{
			Ezptr<AIMScene> Scene = SceneManager::GetScene();
			Ezptr<AIMLayer> Layer = Scene->FindLayer("Default");

			Obj = AIMObject::CreateObject("Obj", Layer);

			Ezptr<AIMTransform> Transform = Obj->GetTransform();

			Transform->SetWorldScale(0.05f, 0.05f, 0.05f);
		}

		Obj->EraseComponent(CT_ANIMATION);
		Animation = Obj->AddComponent<AIMAnimation>("Animation");

		TCHAR TmpName[MAX_PATH] = {};

		lstrcpy(TmpName, Path.GetString());

		TCHAR BoneName[MAX_PATH] = {};

		lstrcpy(BoneName, TmpName);

		int Length = Path.GetLength();

		BoneName[Length - 1] = 'e';
		BoneName[Length - 2] = 'n';
		BoneName[Length - 3] = 'b';

		Animation->LoadBoneFromFullPath(BoneName);

		Animation->AddClip(TmpName);

		const std::list<std::string>* Clips = Animation->GetAddClipList();

		std::list<std::string>::const_iterator StartIter = Clips->begin();
		std::list<std::string>::const_iterator EndIter = Clips->end();

		for (; StartIter != EndIter ; ++StartIter)
		{
			CString ClipName = CA2CT(StartIter->c_str());

			ClipList.AddString(ClipName);
		}

		ClipList.SetCurSel(0);

		Ezptr<AnimationClip> Clip = Animation->GetCurrentClip();


		ClipName = CA2CT(Clip->Name.c_str());
		StartFrame = Clip->StartFrame;
		EndFrame = Clip->EndFrame;
		PlayTime = Clip->PlayTime;
		OptionCombo.SetCurSel(Clip->Option);

		UpdateData(FALSE);
	}
}


void EditFormView::OnBnClickedClipsave()
{
	if (Animation == nullptr)
	{
		return;
	}

	const TCHAR* Filter = TEXT("Animation File(*.anm)|*.anm|모든 파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT(".fbx"), nullptr, OFN_OVERWRITEPROMPT, Filter);

	if (dlg.DoModal() == IDOK)
	{
		CString Path = dlg.GetPathName();

		TCHAR TmpName[MAX_PATH] = {};

		lstrcpy(TmpName, Path.GetString());

		TCHAR BoneName[MAX_PATH] = {};

		lstrcpy(BoneName, TmpName);

		int Length = Path.GetLength();

		BoneName[Length - 1] = 'e';
		BoneName[Length - 2] = 'n';
		BoneName[Length - 3] = 'b';

		Animation->SaveBoneFromFullPath(BoneName);

		Animation->SaveFromFullPath(TmpName);
	}
}


void EditFormView::OnLbnSelchangeCliplist()
{
	if (Animation == nullptr)
	{
		return;
	}

	int Index = ClipList.GetCurSel();

	if (Index == -1)
	{
		return;
	}

	CString ClipName;
	ClipList.GetText(Index, ClipName);

	char TmpName[256] = {};

	WideCharToMultiByte(CP_ACP, 0, ClipName.GetString(), -1, TmpName, ClipName.GetLength(), 0, 0);

	Ezptr<AnimationClip> Clip = Animation->FindClip(TmpName);

	if (Clip == nullptr)
	{
		return;
	}

	ClipName = CA2CT(Clip->Name.c_str());
	StartFrame = Clip->StartFrame;
	EndFrame = Clip->EndFrame;
	PlayTime = Clip->PlayTime;

	OptionCombo.SetCurSel(Clip->Option);

	UpdateData(FALSE);

	Animation->ChangeClip(Clip->Name);
}


void EditFormView::OnBnClickedFramecopy()
{
	if (Animation == nullptr)
	{
		return;
	}

	UpdateData(TRUE);

	if (bFrameCopy == TRUE)
	{
		Animation->GetCurrentKeyFrame(KeyFrameVec);
	}

	else
	{
		KeyFrameVec.clear();
	}
}
