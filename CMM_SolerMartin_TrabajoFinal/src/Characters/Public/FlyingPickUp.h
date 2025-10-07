#pragma once
#include <PlayerProjectile.h>

class FlyingPickUp
{
public:
	FlyingPickUp(const sf::Texture& tex, sf::Vector2f start, sf::Vector2f target, float speed, PlayerProjectileType type);

	bool Update(float dt);

	void Draw(sf::RenderWindow& window);

	PlayerProjectileType GetType() const;

private:
	sf::Sprite sprite;
	sf::Vector2f target;
	float speed = 0.f;
	PlayerProjectileType type;
};