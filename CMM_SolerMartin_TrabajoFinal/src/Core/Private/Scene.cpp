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