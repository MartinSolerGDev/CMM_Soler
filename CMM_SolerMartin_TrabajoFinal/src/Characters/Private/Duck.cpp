#include "Duck.h"
#include "../Public/FileHelpers.h"


Duck::Duck(const sf::Texture& texture, const DuckType& duckType) : Character(texture), currentDuckType(duckType), movementProfile(GetProfileForType(duckType))
{
    sprite.setOrigin({32.f, 32.f });
    sprite.setPosition(sf::Vector2f{ 1280.f / 2.f, 360.f });
    sprite.setRotation(sf::degrees(0.f));
    sprite.setTextureRect(sf::IntRect({ 0,0 }, { 64,64 }));
    sprite.setScale({2.f,2.f});

    velocity.x = (FileHelpers::RandomFloat(0.f, 1.f) > 0.5f) ? 1.f : -1.f;
}

void Duck::Update(float deltaTime)
{
    UpdateMovement(deltaTime);
    UpdateFacingDirection(deltaTime);
    Animate(deltaTime);
}

void Duck::Animate(float deltaTime)
{
    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 2;
        int frameX = currentFrame * 64;
        sprite.setTextureRect(sf::IntRect({ frameX,0},{64,64}));
    }
}

sf::Vector2f Duck::GetDuckSize() const
{
    return sprite.getGlobalBounds().size;
}

void Duck::OnHit()
{
    //todo
}

void Duck::UpdateMovement(float dt)
{
    phaseTimer += dt;
    sf::Vector2f pos = GetPosition();

    switch (phase)
    {
    case DuckPhase::Descend:
        pos.x += movementProfile.horizontalSpeed * dt * velocity.x;
        pos.y += movementProfile.descendSpeed * dt;
        if (phaseTimer >= movementProfile.phaseDuration)
        {
            phase = DuckPhase::Ascend;
            phaseTimer = 0.f;
        }
        break;

    case DuckPhase::Ascend:
        pos.x += movementProfile.horizontalSpeed * dt * velocity.x;
        pos.y -= movementProfile.ascendSpeed * dt;
        if (phaseTimer >= movementProfile.phaseDuration)
        {
            phase = DuckPhase::Escape;
            phaseTimer = 0.f;
        }
        break;

    case DuckPhase::Escape:
        pos.y -= movementProfile.ascendSpeed * 1.5f * dt;
        break;
    }

    float halfWidth = GetDuckSize().x / 2.f;
    if (pos.x <= halfWidth)
    {
        pos.x = halfWidth;
        velocity.x = 1.f; 
    }
    else if (pos.x >= 1280.f - halfWidth)
    {
        pos.x = 1280.f - halfWidth;
        velocity.x = -1.f;
    }

    float halfHeight = GetDuckSize().y / 2.f;
    float minY = 600.f;
    if (pos.y >= minY - halfHeight)
    {
        pos.y = minY - halfHeight;
        if (phase == DuckPhase::Descend)
        {
            phase = DuckPhase::Ascend;
            phaseTimer = 0.f;
        }
    }
    SetPosition(pos);
}

void Duck::UpdateFacingDirection(float dt)
{
    float targetScaleX = (velocity.x < 0.f) ? -2.f : 2.f;

    if (sprite.getScale().x != targetScaleX) 
    {
        sprite.setScale({ targetScaleX, 2});
    }
}

DuckMovementProfile Duck::GetProfileForType(DuckType type)
{
    switch (type)
    {
    case DuckType::Normal:
        return { 130.f,  100.f, 120.f, 6.f };
    case DuckType::Rare:
        return { 160.f, 120.f, 160.f, 4.f };
    case DuckType::Epic:
        return { 200.f, 140.f, 200.f, 3.f };
    case DuckType::Legendary:
        return { 300.f, 160.f, 250.f, 1.3f };
    default:
        return { 100.f, 70.f, 150.f, 2.5f };
    }
}
