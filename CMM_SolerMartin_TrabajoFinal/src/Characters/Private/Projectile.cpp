#include "Projectile.h"


Projectile::Projectile(const sf::Texture& texture) : sprite(texture)
{
}

void Projectile::Activate(const sf::Vector2f& position, const sf::Vector2f& direction)
{
	sprite.setPosition(position);
	velocity = direction * speed;
	bIsActive = true;

	float angleDeg = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f + 90.f;
	sprite.setRotation(sf::degrees(angleDeg));
}

void Projectile::Deactivate()
{
	bIsActive = false; 
}

bool Projectile::IsActive() const
{
	return bIsActive;
}

sf::FloatRect Projectile::GetSpriteBounds() const
{
	return sprite.getGlobalBounds();
}
