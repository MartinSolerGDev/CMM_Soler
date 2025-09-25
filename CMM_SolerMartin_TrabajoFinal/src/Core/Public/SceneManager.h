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

private:
	std::stack<std::unique_ptr<Scene>> scenes;
};

