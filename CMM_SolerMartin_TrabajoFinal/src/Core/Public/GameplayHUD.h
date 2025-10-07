#pragma once
//c++
#include <unordered_map>
//custom
#include <PlayerProjectile.h>


class GameplayHUD
{
public:
    GameplayHUD(sf::Font& font, const std::unordered_map<PlayerProjectileType, sf::Texture*>& bulletTextures, const sf::Texture& buttonTexture);

    void Update(float gameTime, int score, const std::unordered_map<PlayerProjectileType, int>& ammo, PlayerProjectileType currentType);

    void Draw(sf::RenderWindow& window);

    bool IsPauseButtonClicked(const sf::Vector2f& mousePos) const;

    sf::Vector2f GetAmmoIconPosition(PlayerProjectileType type) const;
private:
    sf::Font& font;

    struct AmmoDisplay {
        std::unique_ptr<sf::Sprite> icon;
        std::unique_ptr<sf::Text> count;
    };

    std::unique_ptr<sf::Text> timeText;
    std::unique_ptr<sf::Text> scoreText;

    std::unique_ptr<sf::Sprite> pauseButtonSprite;

    std::unordered_map<PlayerProjectileType, AmmoDisplay> ammoDisplays;

    PlayerProjectileType highlightedType = PlayerProjectileType::Base;
};