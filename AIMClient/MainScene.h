#pragma once

#include "SceneComponent.h"

class MainScene : public SceneComponent
{
public:
	virtual bool Init();

private:
	void CreatePrototype();

public:
	MainScene();
	~MainScene();
};

