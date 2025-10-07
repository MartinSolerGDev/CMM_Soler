#pragma once
//Custom classes
#include "Scene.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "DuckManager.h"

struct Button
{
    std::unique_ptr <sf::Sprite> buttonSprite;
    std::unique_ptr<sf::Text> buttonTxt;
    std::function<void()> onClick;
};

class MenuScene :
    public Scene
{
public:
    MenuScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;
   
    void OnEnterScene() override;
    void OnExitScene() override;

private:
    ResourcesManager& resources;
    SceneManager& manager;

    sf::Font& font;
    
    std::vector<Button> buttons;
    Button CreateButton(const std::string& label, float y, std::function<void()> onClick);



    std::unique_ptr<sf::Text> title;
    std::unique_ptr<sf::Text> enterToContinue;
};

