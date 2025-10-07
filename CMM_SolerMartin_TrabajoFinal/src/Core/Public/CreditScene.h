#pragma once
#include "Scene.h"
#include <MenuScene.h>


class CreditScene :
    public Scene
{
public:
    CreditScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;

    void OnEnterScene() override;
    void OnExitScene() override;

private:
    ResourcesManager& resources;
    SceneManager& manager;
    sf::Font& font;

    std::unique_ptr<sf::Text> title;
    std::vector<std::unique_ptr<sf::Text>> creditsLines;

    std::unique_ptr<sf::Sprite> backSprite;
    std::unique_ptr<sf::Text>   backText;

};

