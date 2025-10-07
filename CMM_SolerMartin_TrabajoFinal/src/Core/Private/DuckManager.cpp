//C++ class

//Custom class
#include "DuckManager.h"
#include "FileHelpers.h"


DuckManager::DuckManager(const std::unordered_map<DuckType, DuckResources>& resources, std::size_t maxDucks,float spawnIntervalSeconds) : resources(resources), maxActiveDucks(maxDucks), spawnInterval(spawnIntervalSeconds)
{
    pool.reserve(maxDucks);

    for (std::size_t i = 0; i < maxDucks; ++i)
    {
        const DuckResources& res = resources.at(DuckType::Normal);

        pool.push_back(DuckSlot{std::make_unique<Duck>(*res.normal, *res.explosion, DuckType::Normal),DuckType::Normal,false});
    }
}


void DuckManager::AddSpawnZone(const SpawnZone& zone)
{
    spawnZones.push_back(zone);
}

void DuckManager::Update(float deltaTime)
{
    // Actualizar patos activos
    for (auto& slot : pool)
    {
        if (!slot.active) continue;

        slot.duck->Update(deltaTime);

        if (slot.duck->IsDead())
        {
            slot.active = false;
            continue;
        }
        float bottomY = slot.duck->GetPosition().y + slot.duck->GetDuckSize().y / 2.f;
        if (bottomY < 0.f)
        {
            slot.active = false;
            scapedDucks += 1;
        }
    }

    elapsedTime += deltaTime;
    spawnInterval = std::max(0.25f, 3.f - elapsedTime * ((3.f - 0.4f) / 300.f));

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

        if (probNormal > 0.4f)    probNormal -= 0.1f;
        if (probRare < 0.3f)    probRare += 0.05f;
        if (probEpic < 0.2f)    probEpic += 0.03f;
        if (probLegendary < 0.1f) probLegendary += 0.02f;

        float total = probNormal + probRare + probEpic + probLegendary;
        probNormal /= total;
        probRare /= total;
        probEpic /= total;
        probLegendary /= total;
    }
}

void DuckManager::Draw(sf::RenderWindow& window)
{
    for (auto& slot : pool)
    {
        if (slot.active) 
        {
            slot.duck->Draw(window);
#ifdef _DEBUG
            sf::FloatRect bounds = slot.duck->GetCollisionBounds();
            sf::RectangleShape box;
            box.setPosition(bounds.position);
            box.setSize(bounds.size);
            box.setFillColor(sf::Color::Transparent);
            box.setOutlineColor(sf::Color::Red);
            box.setOutlineThickness(1.f);
            window.draw(box);
#endif
        }
    }
}

std::vector<DuckManager::DuckSlot>& DuckManager::GetDucks()
{
    return pool;
}

bool DuckManager::hasReachedScapedDucks() const
{
    return scapedDucks > 5;
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
    // Limitar cantidad máxima activa
    std::size_t activeCount = 0;
    for (const auto& slot : pool)
        if (slot.active) ++activeCount;
    if (activeCount >= maxActiveDucks) return;

    // Validar zonas de spawn
    if (spawnZones.empty())
    {
        std::cerr << "No spawn zones defined!\n";
        return;
    }

    // Buscar un slot libre
    for (auto& slot : pool)
    {
        if (!slot.active)
        {
            const auto& zone = spawnZones[FileHelpers::RandomInt(0, static_cast<int>(spawnZones.size()) - 1)];
            sf::Vector2f spawnPos = zone.GetRandomPoint();
            DuckType type = ChooseDuckType();
            auto it = resources.find(type);
            if (it == resources.end() || it->second.normal == nullptr || it->second.explosion == nullptr)
            {
                std::cerr << "Duck resources missing for type!\n";
                return;
            }

            const DuckResources& res = it->second;
            slot.duck = std::make_unique<Duck>(*res.normal, *res.explosion, type);
            slot.duck->SetPosition(spawnPos);

            slot.type = type;
            slot.active = true;
            break;
        }
    }
}
