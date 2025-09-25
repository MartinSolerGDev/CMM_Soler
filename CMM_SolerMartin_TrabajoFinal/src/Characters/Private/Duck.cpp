#include "Duck.h"
#include <random>


Duck::Duck(const sf::Texture& texture) : Character(texture)
{
    sprite.setOrigin({32.f, 32.f });
    sprite.setPosition(sf::Vector2f{ 1280.f / 2.f, 360.f });
    sprite.setRotation(sf::degrees(0.f));
    sprite.setTextureRect(sf::IntRect({ 0,0 }, { 64,64 }));
    sprite.setScale({2.f,2.f});

    //Movement
    velocity = { RandomFloat(-100.f, 100.f), RandomFloat(-250.f, -150.f) };
    changeInterval = RandomFloat(0.5f, 1.5f);
}

void Duck::Update(float deltaTime)
{
    UpdateMovement(deltaTime);
    TryChangeDirection(deltaTime);
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

void Duck::UpdateMovement(float dt)
{
    sf::Vector2f pos = GetPosition();
    pos += velocity * dt;
    SetPosition(pos);
}

void Duck::TryChangeDirection(float dt)
{
    timeSinceLastChange += dt;
    if (timeSinceLastChange >= changeInterval) {
        velocity.x = RandomFloat(-150.f, 150.f);
        velocity.y = RandomFloat(-250.f, -100.f);
        timeSinceLastChange = 0.f;
        changeInterval = RandomFloat(0.5f, 1.5f);
    }
}

void Duck::UpdateFacingDirection(float dt)
{
    float targetScaleX = (velocity.x < 0.f) ? -2.f : 2.f;

    if (sprite.getScale().x != targetScaleX) 
    {
        sprite.setScale({ targetScaleX, 2});
    }
}

float Duck::RandomFloat(float min, float max)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
