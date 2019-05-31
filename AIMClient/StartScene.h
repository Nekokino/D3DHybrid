#pragma once
#include <SceneComponent.h>

class StartScene : public SceneComponent
{
public:
	virtual bool Init();

private:
	void CreatePrototype();
	void StartButton(float _Time);
public:
	StartScene();
	~StartScene();
};

