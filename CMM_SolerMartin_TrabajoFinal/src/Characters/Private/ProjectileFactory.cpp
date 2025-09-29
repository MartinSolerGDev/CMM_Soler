#include "ProjectileFactory.h"

std::unique_ptr<Projectile> ProjectileFactory::CreateProjectile(ProjectileType type, const sf::Texture& texture)
{
    //Add enemy projectile Later
    switch (type)
    {
    case ProjectileType::Player:
        return std::make_unique<PlayerProjectile>(texture, PlayerProjectileType::Base);
    default:
        return nullptr;
    }
}
