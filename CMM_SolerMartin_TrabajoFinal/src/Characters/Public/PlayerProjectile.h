#pragma once
#include "Projectile.h"

enum class PlayerProjectileType
{
    Base,
    Normal,
    Rare,
    Epic,
    Legendary
};


class PlayerProjectile :
    public Projectile
{
public:
    PlayerProjectile(const sf::Texture& texture, PlayerProjectileType projectType);
    ~PlayerProjectile() = default;
    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;
    void ConfigureBullet(PlayerProjectileType type, const sf::Texture& texture);
    sf::FloatRect GetCollisionBounds() const override;

private:
    PlayerProjectileType currentType;
};

