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
    ProjectileManager(const std::unordered_map<PlayerProjectileType, sf::Texture*>& bulletTextures, std::size_t poolSize);

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

    void Fire(const sf::Vector2f& position, const sf::Vector2f& direction);

    void SetCurrentBulletType(PlayerProjectileType type);
    void AddAmmo(PlayerProjectileType type, int amount);

    //Getters
    std::vector<std::unique_ptr<Projectile>>& GetProjectiles();
    const std::unordered_map<PlayerProjectileType, int>& GetAmmo() const;
    PlayerProjectileType GetCurrentType() const;

    bool HasAnyAmmo() const;


private:
    std::vector<std::unique_ptr<Projectile>> pool;
    std::unordered_map<PlayerProjectileType, sf::Texture*> textures;
    std::unordered_map<PlayerProjectileType, int> ammo;
    PlayerProjectileType currentType = PlayerProjectileType::Base;
};

