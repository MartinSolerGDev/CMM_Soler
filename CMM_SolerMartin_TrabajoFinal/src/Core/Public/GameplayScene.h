#pragma once
#include "Scene.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "PlayerCharacter.h"
#include "ProjectileManager.h"
#include "DuckManager.h"

class GameplayScene :
    public Scene
{
public:
    GameplayScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;
   

private:
    ResourcesManager& resources;
    SceneManager& manager;

    std::unique_ptr<PlayerCharacter> player;
    std::unique_ptr<ProjectileManager> projectileManager;
    std::unique_ptr<DuckManager> duckManager;

};

