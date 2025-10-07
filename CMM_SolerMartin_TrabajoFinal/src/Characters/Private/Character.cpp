//C++ class
#include <iostream>
//Custom class
#include "Character.h"


Character::Character(const sf::Texture& tex) : sprite(tex)
{
}

void Character::Update(float deltaTime)
{
    //Override on childrens
}

void Character::Draw(sf::RenderWindow& window) const
{

	window.draw(sprite);

#ifdef _DEBUG
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape box;
    box.setPosition(bounds.position);
    box.setSize(bounds.size);            
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Green);
    box.setOutlineThickness(1.f);
    window.draw(box);
#endif
}
void Character::SetPosition(const sf::Vector2f& position) 
{
    sprite.setPosition(position);
}

void Character::SetRotation(sf::Angle angle) 
{
    sprite.setRotation(angle);
}

void Character::Rotate(sf::Angle offsetAngle) 
{
    sprite.rotate(offsetAngle);
}

sf::Vector2f Character::GetPosition() const 
{
    return sprite.getPosition();
}

sf::Angle Character::GetRotation() const 
{
    return sprite.getRotation(); 
}

sf::FloatRect Character::GetSpriteBounds() const
{
    return sprite.getGlobalBounds();
}

void Character::Animate(float deltaTime)
{
    //Overriten on children
}

