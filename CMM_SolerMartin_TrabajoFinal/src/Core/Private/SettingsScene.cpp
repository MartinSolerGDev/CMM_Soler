#include "SettingsScene.h"
#include "MenuScene.h"

SettingsScene::SettingsScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager, bool pauseMode) : Scene(window), resources(resources), manager(manager), font(resources.GetFont("../assets/font.ttf")), pauseMode(pauseMode)
{
    sf::Vector2f winSize(window.getSize());

    SetBackground(resources.GetTexture("../assets/Background/menubackground.png", sf::IntRect({ 0,0 }, { 1280,720 })));

    title = std::make_unique<sf::Text>(font, pauseMode ? "Pause Menu" : "Settings", 50);
    title->setOrigin(title->getLocalBounds().getCenter());
    title->setPosition({ winSize.x / 2, 80.f });
    title->setFillColor(sf::Color::Black);

    auto& buttonTex = resources.GetTexture("../assets/Background/buttonBackground.png", sf::IntRect({ 0,0 }, { 400,250 }));

    // Crear controles de volumen
    std::vector<std::pair<VolumeType, std::string>> types = {
        {VolumeType::Master, "Master Volume"},
        {VolumeType::Music,  "Music Volume"},
        {VolumeType::SFX,    "SFX Volume"}
    };

    float y = 200.f;
    for (auto& [type, labelText] : types)
    {
        VolumeControl control;
        control.type = type;

        control.label = std::make_unique<sf::Text>(font, labelText, 28);
        control.label->setOrigin(control.label->getLocalBounds().getCenter());
        control.label->setPosition({ winSize.x / 2, y - 40.f });
        control.label->setFillColor(sf::Color::Black);

        control.valueText = std::make_unique<sf::Text>(font, "", 26);
        control.valueText->setOrigin(control.valueText->getLocalBounds().getCenter());
        control.valueText->setPosition({ winSize.x / 2, y });
        control.valueText->setFillColor(sf::Color::Black);

        control.minusSprite = std::make_unique<sf::Sprite>(buttonTex);
        control.minusSprite->setOrigin(control.minusSprite->getGlobalBounds().getCenter());
        control.minusSprite->setScale({ 0.3f, 0.3f });
        control.minusSprite->setPosition({ winSize.x / 2 - 150.f, y });

        control.minusText = std::make_unique<sf::Text>(font, "-", 40);
        control.minusText->setOrigin(control.minusText->getLocalBounds().getCenter());
        control.minusText->setPosition(control.minusSprite->getPosition());
        control.minusText->setFillColor(sf::Color::Black);

        control.plusSprite = std::make_unique<sf::Sprite>(buttonTex);
        control.plusSprite->setOrigin(control.plusSprite->getGlobalBounds().getCenter());
        control.plusSprite->setScale({ 0.3f, 0.3f });
        control.plusSprite->setPosition({ winSize.x / 2 + 150.f, y });

        control.plusText = std::make_unique<sf::Text>(font, "+", 40);
        control.plusText->setOrigin(control.plusText->getLocalBounds().getCenter());
        control.plusText->setPosition(control.plusSprite->getPosition());
        control.plusText->setFillColor(sf::Color::Black);

        UpdateVolumeText(control);

        volumeControls.push_back(std::move(control));
        y += 150.f;
    }
    if (!pauseMode)
    {
        backSprite = std::make_unique<sf::Sprite>(buttonTex);
        backSprite->setOrigin(backSprite->getGlobalBounds().getCenter());
        backSprite->setScale({ 0.5f, 0.4f });
        backSprite->setPosition({ winSize.x / 2, winSize.y - 100.f });

        backText = std::make_unique<sf::Text>(font, "BACK", 30);
        backText->setOrigin(backText->getLocalBounds().getCenter());
        backText->setPosition(backSprite->getPosition());
        backText->setFillColor(sf::Color::Black);
    }
    else
    {
        resumeSprite = std::make_unique<sf::Sprite>(buttonTex);
        resumeSprite->setOrigin(resumeSprite->getGlobalBounds().getCenter());
        resumeSprite->setScale({ 0.6f, 0.5f });
        resumeSprite->setPosition({ winSize.x / 2 - 150.f, winSize.y - 100.f });

        resumeText = std::make_unique<sf::Text>(font, "RESUME", 20);
        resumeText->setOrigin(resumeText->getLocalBounds().getCenter());
        resumeText->setPosition(resumeSprite->getPosition());
        resumeText->setFillColor(sf::Color::Black);

        mainMenuSprite = std::make_unique<sf::Sprite>(buttonTex);
        mainMenuSprite->setOrigin(mainMenuSprite->getGlobalBounds().getCenter());
        mainMenuSprite->setScale({ 0.6f, 0.5f });
        mainMenuSprite->setPosition({ winSize.x / 2 + 150.f, winSize.y - 100.f });

        mainMenuText = std::make_unique<sf::Text>(font, "MAIN MENU", 20);
        mainMenuText->setOrigin(mainMenuText->getLocalBounds().getCenter());
        mainMenuText->setPosition(mainMenuSprite->getPosition());
        mainMenuText->setFillColor(sf::Color::Black);
    }
}

void SettingsScene::HandleEvent(const sf::Event& event)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            for (auto& control : volumeControls)
            {
                if (control.minusSprite && control.minusSprite->getGlobalBounds().contains(worldPos))
                {
                    AudioManager::Get().SetVolume(control.type, AudioManager::Get().GetVolume(control.type) - 10);
                    UpdateVolumeText(control);
                }
                else if (control.plusSprite && control.plusSprite->getGlobalBounds().contains(worldPos))
                {
                    AudioManager::Get().SetVolume(control.type, AudioManager::Get().GetVolume(control.type) + 10);
                    UpdateVolumeText(control);
                }
            }

            if (backSprite && backSprite->getGlobalBounds().contains(worldPos))
            {
                RequestSceneChange("MainMenu");
                return;
            }
            else if (resumeSprite && resumeSprite->getGlobalBounds().contains(worldPos))
            {
                RequestSceneChange("ResumeGameplay");
                return;
            }
            else if (mainMenuSprite && mainMenuSprite->getGlobalBounds().contains(worldPos))
            {
                RequestSceneChange("MainMenu");
                return;
            }

        }
    }

}

void SettingsScene::Update(float deltaTime)
{
    sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    if (backSprite && backText)
    {
        if (backSprite->getGlobalBounds().contains(mousePos))
            backText->setFillColor(sf::Color::Green);
        else
            backText->setFillColor(sf::Color::Black);
    }
    if (resumeSprite && resumeText)
    {
        if (resumeSprite->getGlobalBounds().contains(mousePos))
            resumeText->setFillColor(sf::Color::Green);
        else
            resumeText->setFillColor(sf::Color::Black);
    }

    if (mainMenuSprite && mainMenuText)
    {
        if (mainMenuSprite->getGlobalBounds().contains(mousePos))
            mainMenuText->setFillColor(sf::Color::Green);
        else
            mainMenuText->setFillColor(sf::Color::Black);
    }

    for (auto& control : volumeControls)
    {
        if (control.minusSprite && control.minusText)
        {
            if (control.minusSprite->getGlobalBounds().contains(mousePos))
                control.minusText->setFillColor(sf::Color::Green);
            else
                control.minusText->setFillColor(sf::Color::Black);
        }

        if (control.plusSprite && control.plusText)
        {
            if (control.plusSprite->getGlobalBounds().contains(mousePos))
                control.plusText->setFillColor(sf::Color::Green);
            else
                control.plusText->setFillColor(sf::Color::Black);
        }
    }
}

void SettingsScene::RenderScene()
{
    if (title) window.draw(*title);

    for (auto& control : volumeControls)
    {
        if (control.label) window.draw(*control.label);
        if (control.valueText) window.draw(*control.valueText);
        if (control.minusSprite) window.draw(*control.minusSprite);
        if (control.minusText) window.draw(*control.minusText);
        if (control.plusSprite) window.draw(*control.plusSprite);
        if (control.plusText) window.draw(*control.plusText);
    }

    if (backSprite) { window.draw(*backSprite); window.draw(*backText); }
    if (resumeSprite) { window.draw(*resumeSprite); window.draw(*resumeText); }
    if (mainMenuSprite) { window.draw(*mainMenuSprite); window.draw(*mainMenuText); }
}

void SettingsScene::OnEnterScene()
{
    AudioManager::Get().PlayMusic("MenuMusic", true);
}

void SettingsScene::OnExitScene()
{
}

void SettingsScene::UpdateVolumeText(VolumeControl& control)
{
    int vol = static_cast<int>(AudioManager::Get().GetVolume(control.type));
    control.valueText->setString(std::to_string(vol));
    control.valueText->setOrigin(control.valueText->getLocalBounds().getCenter());
}
