#pragma once
#include "Character.h"

struct DuckMovementProfile
{
    float horizontalSpeed = 0.f;   
    float descendSpeed = 0.f;
    float ascendSpeed = 0.f;
    float phaseDuration = 0.f;
};

enum class DuckType 
{
    Normal,
    Rare,
    Epic,
    Legendary
};

enum class DuckPhase 
{ 
    Descend,
    Ascend,
    Escape
};


class Duck :
    public Character
{
public:
    //Constructor
    Duck(const sf::Texture& texture, const DuckType& duckType);
    ~Duck() override = default;

    //General
    void Update(float deltaTime);
    void Animate(float deltaTime) override; 
    //Getters
    sf::Vector2f  GetDuckSize() const; 

    void OnHit();

private:

    DuckType currentDuckType; 
    DuckPhase phase = DuckPhase::Descend;
    DuckMovementProfile movementProfile;
    float phaseTimer = 0.f;

    //Animations
    float animationTimer = 0.f;
    float animationInterval = 0.2f; 
    int currentFrame = 0;

    //Movement
    sf::Vector2f velocity;
    float timeSinceLastChange = 0.f;
    float changeInterval = 1.f;
    DuckMovementProfile GetProfileForType(DuckType type);

    //Movement funcs
    void UpdateMovement(float dt);
    void UpdateFacingDirection(float dt);
};

