#include "Scene.h"



Scene::Scene(sf::RenderWindow& window) : window(window)
{

}

void Scene::SetBackground(const sf::Texture& texture)
{
    background = std::make_unique<sf::Sprite>(texture);
}
void Scene::Render()
{
    window.clear();
    if (background) 
    {
        window.draw(*background);
    }
    RenderScene();
    window.display();
}

std::string Scene::GetRequestedScene() const
{
    return requestedScene;;
}

void Scene::ClearRequestedScene()
{
    requestedScene.clear();
}

void Scene::RequestSceneChange(const std::string& sceneName)
{
    requestedScene = sceneName;
}
