#include "PlayerProjectile.h"

PlayerProjectile::PlayerProjectile(const sf::Texture& texture) : Projectile(texture)
{
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f });
    sprite.setScale({ 2.f, 2.f });
}

void PlayerProjectile::Update(float deltaTime) 
{
    if (!bIsActive) return;

    sf::Vector2f pos = sprite.getPosition();
    pos += velocity * deltaTime;
    sprite.setPosition(pos);

    // Desactivar si sale de pantalla
    if (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720) 
    {
        Deactivate();
    }
}

void PlayerProjectile::Draw(sf::RenderWindow& window) 
{
    if (bIsActive) window.draw(sprite);
}

