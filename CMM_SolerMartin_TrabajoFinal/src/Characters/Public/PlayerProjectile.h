#pragma once
#include "Projectile.h"
class PlayerProjectile :
    public Projectile
{
public:
    PlayerProjectile(const sf::Texture& texture);
    ~PlayerProjectile() = default;
    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) override;

};

