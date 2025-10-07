#include "GameplayScene.h"
#include "FileHelpers.h"
#include <SettingsScene.h>
#include <LoseScene.h>


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

    bulletTextures = {{PlayerProjectileType::Base, &bulletTex}, {PlayerProjectileType::Normal,&normalBulletTex}, {PlayerProjectileType::Rare, &rareBulletTex}, {PlayerProjectileType::Epic, &epicBulletTex},{PlayerProjectileType::Legendary, &legendaryBulletTex}};
    
    projectileManager = std::make_unique<ProjectileManager>(bulletTextures, 50);
    player->SetProjectileManager(projectileManager.get());

    //Ducks:
    auto& duckNormalTex = resources.GetTexture("../assets/Ducks/duck.png", {});
    auto& duckRareTex = resources.GetTexture("../assets/Ducks/RareDuck.png", {});
    auto& duckEpicTex = resources.GetTexture("../assets/Ducks/EpicDuck.png", {});
    auto& duckLegendaryTex = resources.GetTexture("../assets/Ducks/LegendaryDuck.png", {});

    auto& duckNormalExplosionTex = resources.GetTexture("../assets/Ducks/duckExplosion.png", {});
    auto& duckRareExplosionTex = resources.GetTexture("../assets/Ducks/rareDuckExplosion.png", {});
    auto& duckEpicExplosionTex = resources.GetTexture("../assets/Ducks/epicDuckExplosion.png", {});
    auto& duckLegendaryExplosionTex = resources.GetTexture("../assets/Ducks/legendaryDuckExplosion.png", {});

    std::unordered_map<DuckType, DuckResources> duckResources = {
        {DuckType::Normal,    {&duckNormalTex,    &duckNormalExplosionTex}},
        {DuckType::Rare,      {&duckRareTex,      &duckRareExplosionTex}},
        {DuckType::Epic,      {&duckEpicTex,      &duckEpicExplosionTex}},
        {DuckType::Legendary, {&duckLegendaryTex, &duckLegendaryExplosionTex}}
    };


    duckManager = std::make_unique<DuckManager>(duckResources, 10, 3.f);

    //trees
    AddTree({ 140.f, 400.f }, { 80.f, 350.f }, { 140.f, 500.f });     
    AddTree({ 1040.f, 425.f }, { 1050.f, 350.f }, { 1100.f, 500.f });

    //HUD
    auto& font = resources.GetFont("../assets/font.ttf");
    auto& buttonTex = resources.GetTexture("../assets/Background/Settings.png", sf::IntRect({ 0,0 }, { 400,400 }));

    hud = std::make_unique<GameplayHUD>(font, bulletTextures, buttonTex);
}

void GameplayScene::HandleEvent(const sf::Event& event)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (hud && hud->IsPauseButtonClicked(worldPos))
            {
                manager.PushScene(std::make_unique<SettingsScene>(window, resources, manager, true));
                return;
            }
        }
    }
    if (player)
    {
        player->HandleEvent(event);
    }
}

void GameplayScene::Update(float deltaTime)
{
    if (projectileManager && !projectileManager->HasAnyAmmo())
    {
        manager.PushScene(std::make_unique<LoseScene>(window, resources, manager, score));
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

        if (duckManager->hasReachedScapedDucks())
        {
            manager.PushScene(std::make_unique<LoseScene>(window, resources, manager, score));
            return;
        }
    }
    //Collision check
    for (auto& p : projectileManager->GetProjectiles())
    {
        if (!p->IsActive()) continue;
        for (auto& slot : duckManager->GetDucks())
        {
            if (!slot.active) continue;

            sf::FloatRect projBounds = p->GetCollisionBounds();

            if (projBounds.findIntersection(slot.duck->GetCollisionBounds()))
            {
                p->Deactivate();

                // No desactivar el pato inmediatamente
                if (!slot.duck->IsDying())
                {
                    slot.duck->OnHit(); 

                    auto projType = FileHelpers::DuckTypeToProjectileType(slot.type);

                    // Spawn del pickup visual
                    sf::Vector2f hudPos = hud->GetAmmoIconPosition(projType);
                    pickups.push_back(std::make_unique<FlyingPickUp>(*bulletTextures[projType], slot.duck->GetCollisionBounds().getCenter(), hudPos, 400.f + (400.f * std::min(1.f, gameTime / 300.f)), projType));

                    timeMultiplier = 1.f + (gameTime / 60.f) * 0.25f;
                    score += static_cast<int>(FileHelpers::ScorePerDuck(slot.type) * timeMultiplier);
                }

                break;

            }
        }
    }
    for (auto it = pickups.begin(); it != pickups.end(); )
    {
        if ((*it)->Update(deltaTime))
        {
            projectileManager->AddAmmo((*it)->GetType(), 1);
            it = pickups.erase(it);
        }
        else
        {
            ++it;
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
    
    if(duckManager) duckManager->Draw(window);
    if(player) player->Draw(window);
    if(projectileManager) projectileManager->Draw(window);

    for (auto& tree : treeSprites)
    {
        if (tree.tree)
            window.draw(*tree.tree);
    }
    for (auto& p : pickups)
        if(p)
            p->Draw(window);
    if(hud)
        hud->Draw(window);
}

void GameplayScene::OnEnterScene()
{
    AudioManager::Get().PlayMusic("GameMusic", true);
}

void GameplayScene::OnExitScene()
{
    AudioManager::Get().StopMusic();
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