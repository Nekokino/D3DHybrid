#include "AIMScene.h"
#include "SceneComponent.h"
#include "AIMLayer.h"
#include "AIMObject.h"
#include "AIMCamera.h"
#include "AIMTransform.h"
#include "AIMDevice.h"
#include "AIMRenderer.h"
#include "AIMMaterial.h"
#include "AIMLight.h"

AIMScene::AIMScene()
{
}


AIMScene::~AIMScene()
{
	SceneComList.clear();
	LayerList.clear();

	AIMObject::RemovePrototype(this);
}

Ezptr<AIMObject> AIMScene::GetMainCameraObj() const
{
	return MainCameraObj;
}

Ezptr<AIMCamera> AIMScene::GetMainCamera() const
{
	return MainCamera;
}

Ezptr<AIMTransform> AIMScene::GetMainCameraTransform() const
{
	return MainCameraTransform;
}

bool AIMScene::Init()
{
	AddLayer("Default", 0);
	AddLayer("UI", INT_MAX - 2);

	AddCamera("MainCamera", Vec3(0.0f, 0.0f, -5.0f), Vec3::Zero, CT_PERS, GetDeviceInst->GetResolution().Width, GetDeviceInst->GetResolution().Height, 90.0f, 0.03f, 1000.0f);

	Sky = AIMObject::CreateObject("Sky");

	Sky->SetScene(this);

	Ezptr<AIMTransform> SkyTransform = Sky->GetTransform();

	SkyTransform->SetWorldScale(100000.0f, 100000.0f, 100000.0f);

	Sky->LateUpdate(0.0f);

	Ezptr<AIMRenderer> SkyRenderer = Sky->AddComponent<AIMRenderer>("SkyRenderer");

	SkyRenderer->SetMesh("Sky");
	SkyRenderer->SetRenderState("CullNone");
	SkyRenderer->SetRenderState("LessEqual");

	Ezptr<AIMMaterial> SkyMaterial = Sky->AddComponent<AIMMaterial>("SkyMaterial");

	SkyMaterial->SetDiffuseTextureSet(0, 0, "LinearSampler", 0, 10, "SkyTexture", TEXT("Sky.dds"));

	Sky->Start();

	Ezptr<AIMLayer> DefaultLayer = FindLayer("Default");

	Ezptr<AIMObject> GlobalSpotLightObj = AIMObject::CreateObject("GlobalSpotLight", DefaultLayer);

	Ezptr<AIMTransform> SpotLightTransform = GlobalSpotLightObj->GetTransform();

	SpotLightTransform->SetWorldRotationX(90.0f);
	SpotLightTransform->SetWorldPosition(0.0f, 2.0f, 0.0f);

	Ezptr<AIMLight> GlobalSpotLight = GlobalSpotLightObj->AddComponent<AIMLight>("GlobalSpotLight");

	GlobalSpotLight->SetLightType(LT_SPOT);
	GlobalSpotLight->SetLightDistance(2.0f);
	GlobalSpotLight->SetLightAngle(60.0f, 90.0f);
	GlobalSpotLight->SetLightColor(Vec4::Red, Vec4::Red * 0.2f, Vec4::Red);

	Ezptr<AIMObject> GlobalPointLightObj = AIMObject::CreateObject("GlobalPointLight", DefaultLayer);

	Ezptr<AIMTransform> PointLightTransform = GlobalPointLightObj->GetTransform();

	PointLightTransform->SetWorldPosition(0.0f, -2.0f, 0.0f);

	Ezptr<AIMLight> GlobalPointLight = GlobalPointLightObj->AddComponent<AIMLight>("GlobalPointLight");

	GlobalPointLight->SetLightType(LT_POINT);
	GlobalPointLight->SetLightDistance(2.0f);

	Ezptr<AIMObject> GlobalDirLightObj = AIMObject::CreateObject("GlobalDirLight", DefaultLayer);

	Ezptr<AIMTransform> DirLightTransform = GlobalDirLightObj->GetTransform();

	DirLightTransform->SetWorldRotationX(45.0f);
	DirLightTransform->SetWorldRotationY(90.0f);

	Ezptr<AIMLight> GlobalDirLight = GlobalDirLightObj->AddComponent<AIMLight>("GlobalDirLight");

	GlobalDirLight->SetLightType(LT_DIR);



	return true;
}

int AIMScene::Input(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Input(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Input(_Time);

		++StartIterA;
	}

	return 0;
}

int AIMScene::Update(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Update(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Update(_Time);

		++StartIterA;
	}

	MainCameraObj->Update(_Time);

	return 0;
}

int AIMScene::LateUpdate(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->LateUpdate(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->LateUpdate(_Time);

		++StartIterA;
	}

	MainCameraObj->LateUpdate(_Time);

	return 0;
}

int AIMScene::Collision(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Collision(_Time);

		++StartIter;
	}

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Collision(_Time);

		++StartIterA;
	}

	MainCameraObj->Collision(_Time);

	return 0;
}

int AIMScene::Render(float _Time)
{
	std::list<Ezptr<SceneComponent>>::iterator StartIter = SceneComList.begin();
	std::list<Ezptr<SceneComponent>>::iterator EndIter = SceneComList.end();

	for (; StartIter != EndIter;)
	{
		if ((*StartIter)->IsAlive() == false)
		{
			StartIter = SceneComList.erase(StartIter);
			continue;
		}
		else if ((*StartIter)->IsEnable() == false)
		{
			++StartIter;
			continue;
		}

		(*StartIter)->Render(_Time);

		++StartIter;
	}

	Sky->Render(_Time);

	std::list<Ezptr<AIMLayer>>::iterator StartIterA = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIterA = LayerList.end();

	for (; StartIterA != EndIterA;)
	{
		if ((*StartIterA)->IsAlive() == false)
		{
			StartIterA = LayerList.erase(StartIterA);
			continue;
		}
		else if ((*StartIterA)->IsEnable() == false)
		{
			++StartIterA;
			continue;
		}

		(*StartIterA)->Render(_Time);

		++StartIterA;
	}

	MainCameraObj->Render(_Time);

	return 0;
}

void AIMScene::AddLayer(const std::string & _Name, int _Order)
{
	Ezptr<AIMLayer> Layer = new AIMLayer;

	Layer->SetNameTag(_Name.c_str());
	Layer->Scene = this;

	if (Layer->Init() == false)
	{
		return;
	}

	LayerList.push_back(Layer);

	Layer->SetOrder(_Order);
}

void AIMScene::SortLayer()
{
	LayerList.sort(AIMScene::SortFunc);
}

Ezptr<AIMLayer> AIMScene::FindLayer(const std::string & _Name)
{
	std::list<Ezptr<AIMLayer>>::iterator StartIter = LayerList.begin();
	std::list<Ezptr<AIMLayer>>::iterator EndIter = LayerList.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter)->GetNameTag() == _Name)
		{
			return *StartIter;
		}
	}

	return nullptr;
}

bool AIMScene::AddCamera(const std::string & _Name, const Vec3 & _Pos, const Vec3 & _Rot, CamType _Type, UINT _Widht, UINT _Height, float _Angle, float _Near, float _Far)
{
	Ezptr<AIMObject> CamObj = FindCamera(_Name);

	if (CamObj != nullptr)
	{
		tassertmsg(true, "Camera Name Overlapped");
		return false;
	}

	CamObj = AIMObject::CreateObject(_Name);

	CamObj->SetScene(this);

	Ezptr<AIMTransform> CamTransform = CamObj->GetTransform();

	CamTransform->SetWorldPosition(_Pos);
	CamTransform->SetWorldRotation(_Rot);

	Ezptr<AIMCamera> Cam = CamObj->AddComponent<AIMCamera>(_Name);

	Cam->SetCameraInfo(_Type, _Widht, _Height, _Angle, _Near, _Far);

	if (MainCameraObj == nullptr)
	{
		MainCameraObj = CamObj;

		MainCamera = Cam;

		MainCameraTransform = CamTransform;
	}

	CameraMap.insert(std::unordered_map<std::string, Ezptr<AIMObject>>::value_type(_Name, CamObj));

	return true;
}

bool AIMScene::ChangeCamera(const std::string & _Name)
{
	Ezptr<AIMObject> CamObj = FindCamera(_Name);

	if (CamObj == nullptr)
	{
		tassertmsg(true, "Not exist Camera");
		return false;
	}

	MainCameraObj = CamObj;
	MainCamera = CamObj->FindComponent<AIMCamera>(CT_CAMERA);
	MainCameraTransform = CamObj->GetTransform();

	return true;
}

Ezptr<AIMObject> AIMScene::FindCamera(const std::string & _Name)
{
	std::unordered_map<std::string, Ezptr<AIMObject>>::iterator FindIter = CameraMap.find(_Name);

	if (FindIter == CameraMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool AIMScene::SortFunc(Ezptr<AIMLayer> _Src, Ezptr<AIMLayer> _Dest)
{
	int Src = _Src->GetOrder();
	int Dest = _Dest->GetOrder();

	return Src > Dest;
}
