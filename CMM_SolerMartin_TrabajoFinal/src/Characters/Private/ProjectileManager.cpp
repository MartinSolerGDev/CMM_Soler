#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(const sf::Texture& texture, std::size_t poolSize) : texture(texture)
{
    pool.reserve(poolSize);
    for (std::size_t i = 0; i < poolSize; ++i) 
    {
        pool.push_back(ProjectileFactory::CreateProjectile(ProjectileType::Player, texture));
    }
}

void ProjectileManager::Update(float deltaTime)
{
    for (auto& p : pool) 
    {
        if (p->IsActive()) p->Update(deltaTime);
    }
}

void ProjectileManager::Draw(sf::RenderWindow& window)
{
    for (auto& p : pool) 
    {
        if (p->IsActive()) p->Draw(window);
    }
}

void ProjectileManager::Fire(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    for (auto& p : pool)
    {
        if (!p->IsActive()) 
        {
            p->Activate(position, direction);
            break;
        }
    }
}
