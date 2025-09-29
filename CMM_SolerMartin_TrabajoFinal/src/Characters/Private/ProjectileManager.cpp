#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(const std::unordered_map<PlayerProjectileType, sf::Texture*>& bulletTextures, std::size_t poolSize) : textures(bulletTextures)
{
    pool.reserve(poolSize);
    for (std::size_t i = 0; i < poolSize; ++i)
    {
        pool.push_back(std::make_unique<PlayerProjectile>(*textures.at(PlayerProjectileType::Base), PlayerProjectileType::Base));
    }
    ammo[PlayerProjectileType::Base] = 5;
    ammo[PlayerProjectileType::Normal] = 0;
    ammo[PlayerProjectileType::Rare] = 0;
    ammo[PlayerProjectileType::Epic] = 0;
    ammo[PlayerProjectileType::Legendary] = 0;
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
    if (ammo[currentType] <= 0)
        return;

    for (auto& p : pool)
    {
        if (!p->IsActive())
        {
            auto* proj = dynamic_cast<PlayerProjectile*>(p.get());
            if (proj)
            {
                proj->ConfigureBullet(currentType, *textures.at(currentType));
                proj->Activate(position, direction);

                ammo[currentType]--;
            }
            break;
        }
    }
}

void ProjectileManager::SetCurrentBulletType(PlayerProjectileType type)
{
    if (ammo[type] > 0)
        currentType = type;
}

void ProjectileManager::AddAmmo(PlayerProjectileType type, int amount)
{
    ammo[type] += amount;
}

std::vector<std::unique_ptr<Projectile>>& ProjectileManager::GetProjectiles()
{
    return pool;
}

const std::unordered_map<PlayerProjectileType, int>& ProjectileManager::GetAmmo() const
{
    return ammo;
}

PlayerProjectileType ProjectileManager::GetCurrentType() const
{
    return currentType;
}

bool ProjectileManager::HasAnyAmmo() const
{
    for (auto& [type, count] : ammo)
    {
        if (count > 0)
            return true;
    }
    for (auto& p : pool)
    {
        if (p->IsActive())
            return true;
    }
    return false;
}
