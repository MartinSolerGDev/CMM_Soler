#include "GameplayScene.h"
#include "FileHelpers.h"

GameplayScene::GameplayScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager) 
	: Scene(window), resources(resources), manager(manager)
{
    //Background
    SetBackground(resources.GetTexture("../assets/Background/gameplaybackground.png", sf::IntRect({0,0}, {1280,720})));
    auto& cloudTex = resources.GetTexture("../assets/Background/Cloud.png", {});
    for (int i = 0; i < cloudAmount; ++i)
    {
        sf::Sprite cloud(cloudTex);
        float startX = 250.f + i * FileHelpers::RandomFloat(150.f, 400.f);
        float startY = FileHelpers::RandomFloat(30.f, 200.f); 
        cloud.setPosition({startX, startY});
        float randomScale = FileHelpers::RandomFloat(0.2f, 0.4f);
        cloud.setScale({randomScale, randomScale});
        clouds.push_back(cloud);
    }

    auto& sunTex = resources.GetTexture("../assets/Background/Sun.png", {});
    Sun = std::make_unique<sf::Sprite>(sunTex);
    Sun->setOrigin({sunTex.getSize().x / 2.f, sunTex.getSize().y / 2.f});
    Sun->setPosition({1100.f, 100.f});
    Sun->setScale({0.5f, 0.5f});

    //Gameplay
    auto& playerTex = resources.GetTexture("../assets/player.png", {});
    auto& crosshairTex = resources.GetTexture("../assets/Bullets/crosshair.png", {});
    player = std::make_unique<PlayerCharacter>(window, playerTex, crosshairTex);

    //Bullets
    auto& bulletTex = resources.GetTexture("../assets/Bullets/bullet.png", {});
    auto& normalBulletTex = resources.GetTexture("../assets/Bullets/NormalBullet.png", {});
    auto& rareBulletTex = resources.GetTexture("../assets/Bullets/RareBullet.png", {});
    auto& epicBulletTex = resources.GetTexture("../assets/Bullets/EpicBullet.png", {});
    auto& legendaryBulletTex = resources.GetTexture("../assets/Bullets/LegendaryBullet.png", {});

    std::unordered_map<PlayerProjectileType, sf::Texture*> bulletTextures = {{PlayerProjectileType::Base, &bulletTex}, {PlayerProjectileType::Normal,&normalBulletTex}, {PlayerProjectileType::Rare, &rareBulletTex}, {PlayerProjectileType::Epic, &epicBulletTex},{PlayerProjectileType::Legendary, &legendaryBulletTex}};
    
    projectileManager = std::make_unique<ProjectileManager>(bulletTextures, 50);
    player->SetProjectileManager(projectileManager.get());

    //Ducks:
    auto& duckNormalTex = resources.GetTexture("../assets/Ducks/duck.png", {});
    auto& duckRareTex = resources.GetTexture("../assets/Ducks/RareDuck.png", {});
    auto& duckEpicTex = resources.GetTexture("../assets/Ducks/EpicDuck.png", {});
    auto& duckLegendaryTex = resources.GetTexture("../assets/Ducks/LegendaryDuck.png", {});

    std::unordered_map<DuckType, sf::Texture*> duckTextures = {{DuckType::Normal, &duckNormalTex}, {DuckType::Rare, &duckRareTex},{DuckType::Epic, &duckEpicTex}, {DuckType::Legendary, &duckLegendaryTex}};

    duckManager = std::make_unique<DuckManager>(duckTextures, 10, 3.f);

    //trees
    AddTree({ 140.f, 400.f }, { 80.f, 350.f }, { 140.f, 500.f });     
    AddTree({ 1040.f, 425.f }, { 1050.f, 350.f }, { 1100.f, 500.f });

    //HUD
    auto& font = resources.GetFont("../assets/font.ttf");
    hud = std::make_unique<GameplayHUD>(font, bulletTextures);

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
    if (projectileManager && !projectileManager->HasAnyAmmo())
    {
        
        RequestSceneChange("MainMenu");
        return;
    }

    if (player)
    {
        player->Update(deltaTime);
    }
    if (projectileManager)
    {
        projectileManager->Update(deltaTime);
    }
    if (duckManager)
    {
        duckManager->Update(deltaTime);
    }
    //Collision check
    for (auto& p : projectileManager->GetProjectiles())
    {
        if (!p->IsActive()) continue;
        for (auto& slot : duckManager->GetDucks())
        {
            if (!slot.active) continue;

            sf::FloatRect projBounds = p->GetSpriteBounds();
            sf::FloatRect duckBounds = slot.duck->GetSpriteBounds();

            if (projBounds.findIntersection(duckBounds))
            {
                p->Deactivate();
                slot.active = false; 
                projectileManager->AddAmmo(FileHelpers::DuckTypeToProjectileType(slot.type), 1);
                //slot.duck->OnHit();
                break;
            }
        }
    }

    for (auto& cloud : clouds)
    {
        sf::Vector2f pos = cloud.getPosition();
        pos.x -= cloudSpeed * deltaTime;

        if (pos.x + cloud.getGlobalBounds().size.x < 0.f)
        {
            pos.x = 1280.f + FileHelpers::RandomFloat(10.f, 50.f); 
            pos.y = FileHelpers::RandomFloat(30.f, 200.f);          
        }
        cloud.setPosition(pos);
    }

    if (Sun)
    {
        Sun->rotate(sf::degrees(sunRotationSpeed * deltaTime));

        sunScaleTimer += deltaTime;
        float scale = Sun->getScale().x;

        if (sunGrowing)
        {
            scale += 0.05f * deltaTime;
            if (scale >= 0.5f)
            {
                scale = 0.5f;
                sunGrowing = false;
            }
        }
        else
        {
            scale -= 0.05f * deltaTime;
            if (scale <= 0.4f)
            {
                scale = 0.4f;
                sunGrowing = true;
            }
        }
        Sun->setScale({scale, scale });
    }
    gameTime += deltaTime;
    if(hud)
        hud->Update(gameTime, score, projectileManager->GetAmmo(), projectileManager->GetCurrentType());
}

void GameplayScene::RenderScene()
{
    if (Sun) window.draw(*Sun);
    for (auto& cloud : clouds)
        window.draw(cloud);

    duckManager->Draw(window);
    player->Draw(window);
    projectileManager->Draw(window);

    for (auto& tree : treeSprites)
    {
        if (tree.tree)
            window.draw(*tree.tree);
    }
    if(hud)
        hud->Draw(window);
}

void GameplayScene::AddTree(const sf::Vector2f& pos, const sf::Vector2f& spawnMin, const sf::Vector2f& spawnMax)
{
    auto& treeTexture = resources.GetTexture("../assets/Background/tree.png", {});
    Tree temp;
    temp.tree = std::make_unique<sf::Sprite>(treeTexture);
    temp.spawnZone = sf::FloatRect({spawnMin.x, spawnMin.y}, {spawnMax.x - spawnMin.x, spawnMax.y - spawnMin.y });
    temp.tree->setOrigin(temp.tree->getGlobalBounds().getCenter());
    temp.tree->setPosition(pos);
    treeSprites.push_back(std::move(temp));
    duckManager->AddSpawnZone({ spawnMin, spawnMax });
}