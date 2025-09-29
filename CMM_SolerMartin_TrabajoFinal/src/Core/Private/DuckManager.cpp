//C++ class

//Custom class
#include "DuckManager.h"
#include "FileHelpers.h"

DuckManager::DuckManager(const std::unordered_map<DuckType, sf::Texture*>& duckTextures, std::size_t maxDucks, float spawnIntervalSeconds) : textures(duckTextures), maxActiveDucks(maxDucks), spawnInterval(spawnIntervalSeconds)
{
    pool.reserve(maxDucks);
    for (std::size_t i = 0; i < maxDucks; ++i)
    {
        pool.push_back(DuckSlot{ std::make_unique<Duck>(*textures.at(DuckType::Normal), DuckType::Normal), DuckType::Normal, false });
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

    rarityTimer += deltaTime;
    if (rarityTimer >= rarityIncreaseInterval)
    {
        rarityTimer = 0.f;
        if (probNormal > 0.4f) probNormal -= 0.1f;
        if (probRare < 0.3f) probRare += 0.05f;
        if (probEpic < 0.2f) probEpic += 0.03f;
        if (probLegendary < 0.1f) probLegendary += 0.02f;

        float total = probNormal + probRare + probEpic + probLegendary;
        probNormal /= total;
        probRare /= total;
        probEpic /= total;
        probLegendary /= total;
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

std::vector<DuckManager::DuckSlot>& DuckManager::GetDucks()
{
    return pool;
}

DuckType DuckManager::ChooseDuckType()
{
    float posibility = FileHelpers::RandomFloat(0.f, 1.f);
    if (posibility < probNormal) return DuckType::Normal;
    posibility -= probNormal;
    if (posibility < probRare) return DuckType::Rare;
    posibility -= probRare;
    if (posibility < probEpic) return DuckType::Epic;
    return DuckType::Legendary;
}

void DuckManager::SpawnDuck()
{
    std::size_t activeCount = 0;
    for (const auto& slot : pool)
        if (slot.active) ++activeCount;
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
            const auto& zone = spawnZones[FileHelpers::RandomInt(0, (int)spawnZones.size() - 1)];
            sf::Vector2f spawnPos = zone.GetRandomPoint();
            DuckType type = ChooseDuckType();
            slot.duck = std::make_unique<Duck>(*textures[type], type);
            slot.duck->SetPosition(spawnPos);
            slot.type = type;
            slot.active = true;
            break;
        }
    }
}