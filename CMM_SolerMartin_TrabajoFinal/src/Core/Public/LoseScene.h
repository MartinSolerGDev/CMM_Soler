#pragma once
#include "Scene.h"
#include <ResourcesManager.h>
#include <SceneManager.h>
class LoseScene :
    public Scene
{
public:
    LoseScene(sf::RenderWindow& windows, ResourcesManager& res, SceneManager& sm, int score);

    void OnEnterScene() override;
    void OnExitScene() override;
    void HandleEvent(const sf::Event& event) override;
    void Update(float dt) override;
    void RenderScene() override;

private:
    ResourcesManager& resources;
    SceneManager& sceneManager;
    sf::Font& font;


    std::unique_ptr<sf::Text> title;
    std::unique_ptr<sf::Text> scoreText;

    std::unique_ptr<sf::Sprite> MainMenuSprite;
    std::unique_ptr<sf::Text>   MainMenuText;

    std::unique_ptr<sf::Sprite> PlayAgainSprite;
    std::unique_ptr<sf::Text>   PlayAgainText;

};

