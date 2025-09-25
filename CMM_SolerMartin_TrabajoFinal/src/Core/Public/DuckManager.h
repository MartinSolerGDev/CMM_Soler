#pragma once
//C++
#include <random>
#include <iostream>
#include <vector>
//SFML
#include <SFML/Graphics.hpp>

//Custom
#include "../Public/Duck.h"

struct SpawnZone {
    sf::Vector2f min;
    sf::Vector2f max;

    sf::Vector2f GetRandomPoint() const {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_real_distribution<float> distX(min.x, max.x);
        std::uniform_real_distribution<float> distY(min.y, max.y);
        return { distX(rng), distY(rng) };
    }
};

class DuckManager
{
public:
    DuckManager(const sf::Texture& duckTexture, std::size_t maxDucks, float spawnIntervalSeconds);
    ~DuckManager() = default;

    void AddSpawnZone(const SpawnZone& zone);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    void SpawnDuck();

    struct DuckSlot 
    {
        std::unique_ptr<Duck> duck;
        bool active = false;
    };

    std::vector<DuckSlot> pool;
    const sf::Texture& texture;
    std::size_t maxActiveDucks;

    float spawnInterval = 0.f;
    float spawnTimer = 0.f;

    float spawnMinX = 350.f;
    float spawnMaxX = 1180.f;


    std::vector<SpawnZone> spawnZones;

    static int RandomInt(int min, int max);
};

