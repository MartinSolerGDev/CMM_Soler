#pragma once
//Custom
#include "Character.h"
#include "ProjectileManager.h"

class PlayerCharacter :
    public Character
{
public:
    PlayerCharacter(sf::RenderWindow& win, const sf::Texture& tex, const sf::Texture& crosshairText);
    ~PlayerCharacter() override = default;

    void HandleEvent(const sf::Event& event);
    void Update(float deltaTime) override;
    void Draw(sf::RenderWindow& window) const override;

        //Customs
    void UpdateMovement(float deltaTime);
    void UpdateFacingDir();
    void UpdateAiming();
    void SetProjectileManager(ProjectileManager* manager);

private:
    sf::RenderWindow& window;
    float moveDir = 0.f;


    //Projectile
    ProjectileManager* projectileManager = nullptr;
    sf::Vector2f GetMuzzlePosition() const;

    float fireCooldown = 0.5f;   
    float fireTimer = 0.f;
    int ammo = 20;


    //Crosshair
    sf::Sprite crosshair;
};

