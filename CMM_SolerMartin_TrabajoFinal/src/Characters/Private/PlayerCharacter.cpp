//C++ Class
#include <cmath>
#include <iostream>
//Custom
#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(sf::RenderWindow& win, const sf::Texture& PlayerTex, const sf::Texture& crosshairText) : Character(PlayerTex), window(win), crosshair(crosshairText)
{
    sprite.setOrigin({ PlayerTex.getSize().x / 2.f, PlayerTex.getSize().y / 2.f });
    sprite.setPosition(sf::Vector2f{ 1280.f / 2.f, 720.f - sprite.getGlobalBounds().size.y / 2.f });
    sprite.setScale({ 1.5f, 1.5f });

    crosshair.setOrigin({ crosshairText.getSize().x / 2.f, crosshairText.getSize().y / 2.f });
}

void PlayerCharacter::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) 
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::A ||
            keyPressed->scancode == sf::Keyboard::Scancode::Left)
        {
            moveDir = -1.f;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::D ||
            keyPressed->scancode == sf::Keyboard::Scancode::Right)
        {
            moveDir = 1.f;
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
            projectileManager->SetCurrentBulletType(PlayerProjectileType::Base);
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
            projectileManager->SetCurrentBulletType(PlayerProjectileType::Normal);
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
            projectileManager->SetCurrentBulletType(PlayerProjectileType::Rare);
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
            projectileManager->SetCurrentBulletType(PlayerProjectileType::Epic);
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
            projectileManager->SetCurrentBulletType(PlayerProjectileType::Legendary);
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) 
    {
        if (keyReleased->scancode == sf::Keyboard::Scancode::A ||
            keyReleased->scancode == sf::Keyboard::Scancode::Left ||
            keyReleased->scancode == sf::Keyboard::Scancode::D ||
            keyReleased->scancode == sf::Keyboard::Scancode::Right)
        {
            moveDir = 0.f;
        }
    }
    else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) 
    {
        if (mousePressed->button == sf::Mouse::Button::Left) 
        {
            if (fireTimer == 0.f)
            {
                sf::Vector2f muzzle = GetMuzzlePosition();
                sf::Vector2f mouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                sf::Vector2f dir = mouse - muzzle;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len != 0.f) dir /= len;

                if (projectileManager)
                {
                    projectileManager->Fire(muzzle, dir);

                    fireTimer = fireCooldown;
                }
            }

        }
    }
}

void PlayerCharacter::Update(float deltaTime)
{
    UpdateMovement(deltaTime);
    UpdateFacingDir();
    UpdateAiming();
}

void PlayerCharacter::Draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
    window.draw(crosshair);
}

void PlayerCharacter::UpdateMovement(float deltaTime)
{
    //Update crosshair
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    crosshair.setPosition(mousePos);

    //cd updt
    fireTimer -= deltaTime;
    if (fireTimer < 0.f) fireTimer = 0.f;

    //Updatet character movement
    if (moveDir == 0.f) return;
    sf::Vector2f pos = GetPosition();
    float newX = pos.x + moveDir * moveSpeed * deltaTime;
    float halfWidth = sprite.getGlobalBounds().size.x / 2.f;
    newX = std::clamp(newX, 0.f + halfWidth, 1280.f - halfWidth);
    SetPosition(sf::Vector2f{ newX, pos.y });
}

void PlayerCharacter::UpdateFacingDir()
{
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    sf::Vector2f pos = GetPosition();
    float flipThreshold = 10.f;
    if (mousePos.x < pos.x - flipThreshold) 
    {
        sprite.setScale(sf::Vector2f{ -2.f, 2.f });
    }
    else if (mousePos.x > pos.x + flipThreshold) 
    {
        sprite.setScale(sf::Vector2f{ 2.f, 2.f });
    }
}

void PlayerCharacter::UpdateAiming()
{
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    sf::Vector2f pos = GetPosition();
    float dx = mousePos.x - pos.x;
    float dy = mousePos.y - pos.y;
    bool isFlipped = sprite.getScale().x < 0.f;

    if (dy > 0.f) 
    {
        SetRotation(sf::degrees(0.f));
    }
    else 
    {
        float angleDeg = std::atan2(dy, dx) * 180.f / 3.14159265f;
        if (isFlipped) 
        {
            angleDeg += 180.f;
        }
        //angleDeg -= 90.f;
        SetRotation(sf::degrees(angleDeg));
    }
}

void PlayerCharacter::SetProjectileManager(ProjectileManager* manager)
{
    projectileManager = manager;
}

sf::Vector2f PlayerCharacter::GetMuzzlePosition() const
{
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f localOffset(32.f, -4.f); 

    bool isFlipped = sprite.getScale().x < 0.f;
    if (isFlipped)
    {
        localOffset.x = -localOffset.x;
    }
    float angleRad = sprite.getRotation().asRadians();

    sf::Vector2f rotatedOffset;
    rotatedOffset.x = localOffset.x * std::cos(angleRad) - localOffset.y * std::sin(angleRad);
    rotatedOffset.y = localOffset.x * std::sin(angleRad) + localOffset.y * std::cos(angleRad);

    return pos + rotatedOffset;
}

