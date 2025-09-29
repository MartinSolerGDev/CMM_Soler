#include "GameplayHUD.h"
#include <string>

GameplayHUD::GameplayHUD(sf::Font& f,
    const std::unordered_map<PlayerProjectileType, sf::Texture*>& bulletTextures)
    : font(f)
{
    timeText = std::make_unique<sf::Text>(font, "Time: 0", 20);
    scoreText = std::make_unique<sf::Text>(font, "Score: 0", 20);

    timeText->setPosition({ 1000.f, 670.f });
    scoreText->setPosition({ 1120.f, 670.f });

    float x = 20.f;
    float y = 670.f;
    for (auto& [type, tex] : bulletTextures)
    {
        AmmoDisplay display;
        display.icon = std::make_unique<sf::Sprite>(*tex);
        display.icon->setScale({2.5f, 2.5f});
        display.icon->setOrigin({ tex->getSize().x / 2.f, tex->getSize().y / 2.f });
        if (type != PlayerProjectileType::Base)
        {
            display.icon->setRotation(sf::degrees(180.f));
        }
        display.icon->setPosition({x, y });
        display.count = std::make_unique<sf::Text>(font, "0", 30);
        display.count->setPosition({x, y + tex->getSize().y / 2.f });

        ammoDisplays[type] = std::move(display);
        x += 75.f;
    }
}

void GameplayHUD::Update(float gameTime, int score, const std::unordered_map<PlayerProjectileType, int>& ammo, PlayerProjectileType currentType)
{
    highlightedType = currentType;

    int seconds = static_cast<int>(gameTime);
    timeText->setString("Time: " + std::to_string(seconds) + "s");
    scoreText->setString("Score: " + std::to_string(score));

    for (auto& [type, display] : ammoDisplays)
    {
        int count = 0;
        if (auto it = ammo.find(type); it != ammo.end())
            count = it->second;

        display.count->setString(std::to_string(count));

        if (type == highlightedType)
        {
            display.count->setFillColor(sf::Color::Yellow);
            display.icon->setColor(sf::Color::Yellow);
        }
        else
        {
            display.count->setFillColor(sf::Color::White);
            display.icon->setColor(sf::Color::White);
        }
    }
}

void GameplayHUD::Draw(sf::RenderWindow& window)
{
    for (auto& [_, display] : ammoDisplays)
    {
        if (display.icon && display.count)
        {
            window.draw(*display.icon);
            window.draw(*display.count);
        }
    }
    if(timeText)
        window.draw(*timeText);
    if(scoreText)
        window.draw(*scoreText);
}
