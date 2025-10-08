#include "CreditScene.h"
#include <AudioManager.h>

CreditScene::CreditScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager) : Scene(window), resources(resources), manager(manager), font(resources.GetFont("res/assets/font.ttf"))
{
    sf::Vector2f winSize(window.getSize());
    SetBackground(resources.GetTexture("res/assets/Background/menubackground.png",sf::IntRect({ 0,0 }, { 1280,720 })));


    title = std::make_unique<sf::Text>(font, "Credits", 50);
    title->setFillColor(sf::Color::Black);
    title->setOrigin(title->getLocalBounds().getCenter());
    title->setPosition({ winSize.x / 2, 100.f });


    std::vector<std::string> lines = {
       "Programming: Martin Soler",
       "Art - Weapon: CosmicOnion",
       "Art - Backgrounds, Bullets & Special Ducks: Celeste Saudejaud",
       "Art - Normal Duck: Duckhive",
       "SFX - Mastersoundboy2005, Michel Hollicardo, TampaJoey",
       "Music - chajamakesmusic",
       "Special Thanks:",
       "Franco Fioretti",
       "Federico Olive",
       "Nicolas Mironoff"
    };


    float y = 150.f;
    for (auto& line : lines)
    {
        auto txt = std::make_unique<sf::Text>(font, line, 30);
        txt->setFillColor(sf::Color::Black);
        txt->setOrigin(txt->getLocalBounds().getCenter());
        txt->setPosition({ winSize.x / 2, y });
        y += 35.f;
        creditsLines.push_back(std::move(txt));
    }

    auto& buttonTex = resources.GetTexture("res/assets/Background/buttonBackground.png",sf::IntRect({ 0,0 }, { 400,250 }));

    backSprite = std::make_unique<sf::Sprite>(buttonTex);
    backSprite->setOrigin(backSprite->getGlobalBounds().getCenter());
    backSprite->setScale({ 0.5f, 0.4f });
    backSprite->setPosition({ winSize.x / 2, winSize.y - 150.f });

    backText = std::make_unique<sf::Text>(font, "BACK", 30);
    backText->setFillColor(sf::Color::Black);
    backText->setOrigin(backText->getLocalBounds().getCenter());
    backText->setPosition(backSprite->getPosition());
}

void CreditScene::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            RequestSceneChange("MainMenu");
            return;
        }
    }
    else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            if (backSprite->getGlobalBounds().contains(worldPos))
            {
                RequestSceneChange("MainMenu");
                return;
            }
        }
    }
}

void CreditScene::Update(float deltaTime)
{
    sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    if (backSprite && backSprite->getGlobalBounds().contains(mousePos))
        backText->setFillColor(sf::Color::Green);
    else
        backText->setFillColor(sf::Color::Black);

}

void CreditScene::RenderScene()
{
    if (title) window.draw(*title);
    for (auto& line : creditsLines) window.draw(*line);

    if (backSprite) window.draw(*backSprite);
    if (backText)   window.draw(*backText);

}

void CreditScene::OnEnterScene()
{
    AudioManager::Get().PlayMusic("MenuMusic", true);
}

void CreditScene::OnExitScene()
{
}
