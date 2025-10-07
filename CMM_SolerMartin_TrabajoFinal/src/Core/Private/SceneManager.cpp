#include "SceneManager.h"

void SceneManager::PushScene(std::unique_ptr<Scene> scene)
{
    if (!scenes.empty())
        scenes.top()->OnExitScene();   

    if(scene)
        scenes.push(std::move(scene));

    if (!scenes.empty())
        scenes.top()->OnEnterScene();  
}

void SceneManager::PopScene()
{
    if (!scenes.empty())
    {
        scenes.top()->OnExitScene();  
        scenes.pop();

        if (!scenes.empty())
            scenes.top()->OnEnterScene(); 
    }
}

Scene* SceneManager::GetCurrentScene()
{
    return scenes.empty() ? nullptr : scenes.top().get();
}

bool SceneManager::hasScene() const
{
    return !scenes.empty();
}

void SceneManager::SwitchScene(std::unique_ptr<Scene> scene)
{
    if (!scenes.empty())
    {
        scenes.top()->OnExitScene();  
        scenes.pop();
    }
    if(scene)
        scenes.push(std::move(scene));

    if (!scenes.empty())
        scenes.top()->OnEnterScene();  
}

