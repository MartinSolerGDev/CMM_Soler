#include "GameplayScene.h"

GameplayScene::GameplayScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager) 
	: Scene(window), resources(resources), manager(manager)
{
    SetBackground(resources.GetTexture("../assets/gameplaybackground.png", sf::IntRect({0,0}, {1280,720})));

    auto& playerTex = resources.GetTexture("../assets/player.png", {});
    auto& bulletTex = resources.GetTexture("../assets/bullet.png", {});
    auto& crosshairTex = resources.GetTexture("../assets/crosshair.png", {});
    auto& duckTex = resources.GetTexture("../assets/duck.png", {});

    player = std::make_unique<PlayerCharacter>(window, playerTex, crosshairTex);
    projectileManager = std::make_unique<ProjectileManager>(bulletTex, 20);
    player->SetProjectileManager(projectileManager.get());

    duckManager = std::make_unique<DuckManager>(duckTex, 10, 3.f);
    duckManager->AddSpawnZone({ {80.f, 350.f}, {140.f, 500.f} }); // Left tree
    duckManager->AddSpawnZone({ {1050.f, 350.f}, {1100.f, 500.f} }); // Right tree
}

void GameplayScene::HandleEvent(const sf::Event& event)
{
    if (player)
    {
        player->HandleEvent(event);
    }
}

void GameplayScene::Update(float deltaTime)
{
    if (player)
    {
        player->Update(deltaTime);
    }
    if (duckManager)
    {
        duckManager->Update(deltaTime);
    }
    if (projectileManager)
    {
        projectileManager->Update(deltaTime);
    }
}

void GameplayScene::RenderScene()
{
    duckManager->Draw(window);
    player->Draw(window);
    projectileManager->Draw(window);
}


