#pragma once
//C++
#include <vector>
#include <memory>
//SFML
#include <SFML/Graphics.hpp>
//Custom
#include "Projectile.h"
#include "ProjectileFactory.h"

class ProjectileManager
{
public:
    ProjectileManager(const sf::Texture& texture, std::size_t poolSize);

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void Fire(const sf::Vector2f& position, const sf::Vector2f& direction);

private:
    std::vector<std::unique_ptr<Projectile>> pool;
    const sf::Texture& texture;

};

