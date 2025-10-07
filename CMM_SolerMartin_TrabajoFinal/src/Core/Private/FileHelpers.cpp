#include "FileHelpers.h"

float FileHelpers::RandomFloat(float min, float max)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

int FileHelpers::RandomInt(int min, int max)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

bool FileHelpers::RandomChance(float probability)
{
    return RandomFloat(0.f, 1.f) < probability;
}

PlayerProjectileType FileHelpers::DuckTypeToProjectileType(DuckType duckType)
{
    switch (duckType)
    {
    case DuckType::Normal:    return PlayerProjectileType::Normal;
    case DuckType::Rare:      return PlayerProjectileType::Rare;
    case DuckType::Epic:      return PlayerProjectileType::Epic;
    case DuckType::Legendary: return PlayerProjectileType::Legendary;
    default:                  return PlayerProjectileType::Base;
    }

}

int FileHelpers::ScorePerDuck(DuckType duckType)
{
    switch (duckType)
    {
    case DuckType::Normal:    return 25;
    case DuckType::Rare:      return 50;
    case DuckType::Epic:      return 100;
    case DuckType::Legendary: return 150;
    default:                  return 0;
    }
}
