#include "FlyingPickUp.h"

FlyingPickUp::FlyingPickUp(const sf::Texture& tex, sf::Vector2f start, sf::Vector2f target, float speed, PlayerProjectileType type) :sprite(tex), target(target), speed(speed), type(type)
{
    sprite.setOrigin(sprite.getGlobalBounds().getCenter());
    sprite.setPosition(start);
    sprite.setScale({ 3.f, 3.f });
}

bool FlyingPickUp::Update(float dt)
{
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = target - pos;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist < 5.f) 
        return true;

    dir /= dist;
    pos += dir * speed * dt;
    sprite.setPosition(pos);
    return false;

}

void FlyingPickUp::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

PlayerProjectileType FlyingPickUp::GetType() const
{
	return type;
}
