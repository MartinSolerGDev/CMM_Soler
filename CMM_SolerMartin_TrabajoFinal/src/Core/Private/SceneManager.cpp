#include "SceneManager.h"

void SceneManager::PushScene(std::unique_ptr<Scene> scene)
{
	scenes.push(std::move(scene));
}

void SceneManager::PopScene()
{
	if (!scenes.empty()) scenes.pop();
}

Scene* SceneManager::GetCurrentScene()
{
	return scenes.empty() ? nullptr : scenes.top().get();
}

bool SceneManager::hasScene() const
{
	return !scenes.empty();
}
