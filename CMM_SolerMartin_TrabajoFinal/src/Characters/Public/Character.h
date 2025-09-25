#pragma once
#include <SFML/Graphics.hpp>

class Character
{
public:
    Character(const sf::Texture& tex);
    virtual ~Character() = default;

    virtual void Update(float deltaTime);
    virtual void Draw(sf::RenderWindow& window) const;

    // Transforms
    void SetPosition(const sf::Vector2f& position);
    void SetRotation(sf::Angle angle);
    void Rotate(sf::Angle offsetAngle);

    //Getters
    sf::Vector2f GetPosition() const;
    sf::Angle GetRotation() const;

    //Animations
    virtual void Animate(float deltaTime);
protected:
    sf::Sprite sprite;
    float moveSpeed = 200.f;
};

