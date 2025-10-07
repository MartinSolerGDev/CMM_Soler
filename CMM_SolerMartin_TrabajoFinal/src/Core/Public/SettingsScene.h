#pragma once
#include "Scene.h"
#include <ResourcesManager.h>
#include <SceneManager.h>
#include <AudioManager.h>

struct VolumeControl {
    VolumeType type = VolumeType::Master;
    std::unique_ptr<sf::Text> label;
    std::unique_ptr<sf::Text> valueText;
    std::unique_ptr<sf::Sprite> minusSprite;
    std::unique_ptr<sf::Text> minusText;
    std::unique_ptr<sf::Sprite> plusSprite;
    std::unique_ptr<sf::Text> plusText;
};

class SettingsScene :
    public Scene
{
public:
    SettingsScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager, bool pauseMode = false);

    void HandleEvent(const sf::Event& event) override;
    void Update(float deltaTime) override;
    void RenderScene() override;

    void OnEnterScene() override;
    void OnExitScene() override;

private:
    ResourcesManager& resources;
    SceneManager& manager;
    sf::Font& font;

    bool pauseMode = false;

    std::unique_ptr<sf::Text> title;


    std::vector<VolumeControl> volumeControls;

    // Botones de navegación
    std::unique_ptr<sf::Sprite> backSprite;
    std::unique_ptr<sf::Text>   backText;

    std::unique_ptr<sf::Sprite> resumeSprite;
    std::unique_ptr<sf::Text>   resumeText;

    std::unique_ptr<sf::Sprite> mainMenuSprite;
    std::unique_ptr<sf::Text>   mainMenuText;

    void UpdateVolumeText(VolumeControl& control);
};

