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

struct DuckResources {
    sf::Texture* normal;
    sf::Texture* explosion;
};


class DuckManager
{
public:
    DuckManager(const std::unordered_map<DuckType, DuckResources>& resources, std::size_t maxDucks, float spawnIntervalSeconds);


    ~DuckManager() = default;

    void AddSpawnZone(const SpawnZone& zone);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
   
    struct DuckSlot
    {
        std::unique_ptr<Duck> duck;
        DuckType type = DuckType::Normal;
        bool active = false;
    };

    std::vector<DuckManager::DuckSlot>& GetDucks();

    bool hasReachedScapedDucks() const;

private:
    DuckType ChooseDuckType();

    void SpawnDuck();

    std::vector<DuckSlot> pool;
    std::unordered_map<DuckType, DuckResources> resources;

    std::size_t maxActiveDucks;

    float spawnInterval = 0.f;
    float spawnTimer = 0.f;
    float elapsedTime = 0.f;

    float spawnMinX = 350.f;
    float spawnMaxX = 1180.f;

    std::vector<SpawnZone> spawnZones;

    //Spawn probabilities:
    float rarityTimer = 0.f;
    float rarityIncreaseInterval = 10.f;

    float probNormal = 1.f;
    float probRare = 0.f;
    float probEpic = 0.f;
    float probLegendary = 0.f;



    int scapedDucks = 0;
};

