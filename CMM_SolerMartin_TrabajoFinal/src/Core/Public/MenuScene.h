#pragma once
//Custom classes
#include "Scene.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "DuckManager.h"
class MenuScene :
    public Scene
{
public:
    MenuScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;
   

private:
    ResourcesManager& resources;
    SceneManager& manager;

    sf::Font& font;
    
    std::unique_ptr <sf::Sprite> buttonSprite;
    std::unique_ptr<sf::Text> title;
    std::unique_ptr<sf::Text> buttonTxt;
    std::unique_ptr<sf::Text> enterToContinue;
};

