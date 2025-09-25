#pragma once
//C++
#include <memory>

//Custom
#include "Projectile.h"
#include "PlayerProjectile.h"

enum class ProjectileType 
{
	Player,
	Enemy
};
class ProjectileFactory
{
public:
	static std::unique_ptr<Projectile> CreateProjectile(ProjectileType type, const sf::Texture& texture);
};

