#pragma once
#include "Scene.h"
#include "ResourcesManager.h"
#include "SceneManager.h"
#include "PlayerCharacter.h"
#include "ProjectileManager.h"
#include "DuckManager.h"
#include <GameplayHUD.h>
#include <../Public/FlyingPickUp.h>

struct Tree {
    std::unique_ptr <sf::Sprite> tree;
    sf::FloatRect spawnZone;
};

class GameplayScene :
    public Scene
{
public:
    GameplayScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;
   
    void OnEnterScene() override;
    void OnExitScene() override;

private:
    ResourcesManager& resources;
    SceneManager& manager;

    //Gameplay
    std::unique_ptr<PlayerCharacter> player;
    std::unique_ptr<ProjectileManager> projectileManager;
    std::unique_ptr<DuckManager> duckManager;

    //Animations
    std::vector<std::unique_ptr<FlyingPickUp>> pickups;

    //PlayerHUD
    std::unique_ptr<GameplayHUD> hud;
    float gameTime = 0.f;
    int score = 0;
    float timeMultiplier = 0;
    //background animated assets
    void AddTree(const sf::Vector2f& pos, const sf::Vector2f& spawnMin, const sf::Vector2f& spawnMax);

    std::unordered_map<PlayerProjectileType, sf::Texture*> bulletTextures;

    std::vector<Tree> treeSprites;
    std::vector<sf::Sprite> clouds;
    std::unique_ptr <sf::Sprite> Sun;

    float cloudAmount = 10;
    float cloudSpeed = 40.f;
    float spacing = 150.f;
    float sunRotationSpeed = 5.f; 
    float sunScaleTimer = 0.f;
    bool sunGrowing = true;

};

