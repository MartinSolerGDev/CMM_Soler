#include "PlayerProjectile.h"

PlayerProjectile::PlayerProjectile(const sf::Texture& texture, PlayerProjectileType projectType) : Projectile(texture), currentType(projectType)
{
    sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f });
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

void PlayerProjectile::ConfigureBullet(PlayerProjectileType type, const sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setOrigin({ texture.getSize().x / 2.f, texture.getSize().y / 2.f });

    switch (type)
    {
    case PlayerProjectileType::Base:
        speed = 600.f;
        sprite.setScale({ 2.f, 2.f });
        break;

    case PlayerProjectileType::Normal:
        speed = 750.f;
        sprite.setScale({ 3.f, 3.f });
        sprite.setRotation(sf::degrees(180.f));
        break;
    case PlayerProjectileType::Rare:
        speed = 1000.f;
        sprite.setScale({ 4.5f, 4.5f });
        sprite.setRotation(sf::degrees(180.f));
        break;
    case PlayerProjectileType::Epic:
        speed = 2000.f;
        sprite.setScale({ 2.5f, 2.5f });
        sprite.setRotation(sf::degrees(180.f));
        break;
    case PlayerProjectileType::Legendary:
        speed = 1250.f;
        sprite.setScale({ 10.f, 10.f });
        sprite.setRotation(sf::degrees(180.f));
        break;
    }
}
