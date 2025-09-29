#pragma once
//C++
#include <memory>
#include <stack>

//Custom
#include "Scene.h"

class SceneManager
{
public:
	void PushScene(std::unique_ptr<Scene> scene);
	void PopScene();
	Scene* GetCurrentScene();
	bool hasScene() const;

	void SwitchScene(std::unique_ptr<Scene> scene);

private:
	std::stack<std::unique_ptr<Scene>> scenes;
};

