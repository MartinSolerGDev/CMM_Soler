#pragma once
#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(const sf::Texture& texture);
	virtual ~Projectile() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;

    virtual void Activate(const sf::Vector2f& position, const sf::Vector2f& direction);
    virtual void Deactivate();
    bool IsActive() const;

    sf::FloatRect GetSpriteBounds() const;

protected:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool bIsActive = false;
    float speed = 600.f;

};

