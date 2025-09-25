#pragma once
#include "Character.h"
class Duck :
    public Character
{
public:
    //Constructor
    Duck(const sf::Texture& texture);
    ~Duck() override = default;

    //General
    void Update(float deltaTime);
    void Animate(float deltaTime) override; 
    //Getters
    sf::Vector2f  GetDuckSize() const; 

private:
    //Animations
    float animationTimer = 0.f;
    float animationInterval = 0.2f; 
    int currentFrame = 0;

    //Movement
    sf::Vector2f velocity;
    float timeSinceLastChange = 0.f;
    float changeInterval = 1.f;
    
    //Movement funcs
    void UpdateMovement(float dt);
    void TryChangeDirection(float dt);
    void UpdateFacingDirection(float dt);

    //Helpers
    static float RandomFloat(float min, float max);

};

