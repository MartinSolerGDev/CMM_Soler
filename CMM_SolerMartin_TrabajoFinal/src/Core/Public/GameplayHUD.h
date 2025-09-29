#pragma once
//c++
#include <unordered_map>
//custom
#include <PlayerProjectile.h>


class GameplayHUD
{
public:
    GameplayHUD(sf::Font& font, const std::unordered_map<PlayerProjectileType, sf::Texture*>& bulletTextures);
    void Update(float gameTime, int score, const std::unordered_map<PlayerProjectileType, int>& ammo, PlayerProjectileType currentType);

    void Draw(sf::RenderWindow& window);
private:
    sf::Font& font;

    struct AmmoDisplay {
        std::unique_ptr<sf::Sprite> icon;
        std::unique_ptr<sf::Text> count;
    };

    std::unique_ptr<sf::Text> timeText;
    std::unique_ptr<sf::Text> scoreText;

    std::unordered_map<PlayerProjectileType, AmmoDisplay> ammoDisplays;

    PlayerProjectileType highlightedType = PlayerProjectileType::Base;
};