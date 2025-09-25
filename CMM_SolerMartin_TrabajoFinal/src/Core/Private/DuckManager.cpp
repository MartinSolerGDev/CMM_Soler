//C++ class

//Custom class
#include "DuckManager.h"

DuckManager::DuckManager(const sf::Texture& duckTexture, std::size_t maxDucks, float spawnIntervalSeconds)
    : texture(duckTexture), maxActiveDucks(maxDucks), spawnInterval(spawnIntervalSeconds)
{
    pool.reserve(maxDucks);
    for (std::size_t i = 0; i < maxDucks; ++i)
    {
        pool.push_back(DuckSlot{ std::make_unique<Duck>(texture) });
    }
}

void DuckManager::AddSpawnZone(const SpawnZone& zone)
{
    spawnZones.push_back(zone);
}

void DuckManager::Update(float deltaTime)
{
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) 
    {
        spawnTimer = 0.f;
        SpawnDuck();
    }

    for (auto& slot : pool) 
    {
        if (slot.active) 
        {
            slot.duck->Update(deltaTime);
            float bottomY = slot.duck->GetPosition().y + slot.duck->GetDuckSize().y / 2.f;
            if (bottomY < 0.f) 
            {
                slot.active = false;
            }
        }
    }
}

void DuckManager::Draw(sf::RenderWindow& window)
{
    for (auto& slot : pool)
    {
        if (slot.active) 
        {
            slot.duck->Draw(window);
        }
    }
}

void DuckManager::SpawnDuck()
{
    std::size_t activeCount = 0;
    for (const auto& slot : pool) 
    {
        if (slot.active) ++activeCount;
    }
    if (activeCount >= maxActiveDucks) return;
    for (auto& slot : pool) 
    {
        if (!slot.active)
        {
            if (spawnZones.empty())
            {
                std::cerr << "No spawn zones defined!\n";
                return;
            }
            const auto& zone = spawnZones[RandomInt(0, (int)spawnZones.size() - 1)];
            sf::Vector2f spawnPos = zone.GetRandomPoint();
            slot.duck->SetPosition(spawnPos);
            slot.active = true;
            break;
        }
    }
}

int DuckManager::RandomInt(int min, int max)
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}
