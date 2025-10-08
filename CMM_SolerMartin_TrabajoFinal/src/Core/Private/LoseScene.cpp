#include "LoseScene.h"
#include <AudioManager.h>
#include "GameplayScene.h"
#include "MenuScene.h"

LoseScene::LoseScene(sf::RenderWindow& windows, ResourcesManager& res, SceneManager& sm, int score) : Scene(windows), resources(res), sceneManager(sm), font(resources.GetFont("res/assets/font.ttf"))
{
    sf::Vector2f winSize(window.getSize());

    SetBackground(resources.GetTexture("res/assets/Background/menubackground.png",
        sf::IntRect({ 0,0 }, { 1280,720 })));

    auto& buttonTex = resources.GetTexture("res/assets/Background/buttonBackground.png",sf::IntRect({ 0,0 }, { 400,250 }));

    title = std::make_unique<sf::Text>(font, "YOU LOSE!", 80);
    title->setFillColor(sf::Color::Red);
    title->setOrigin(title->getGlobalBounds().getCenter());
    title->setPosition({winSize.x / 2.f, 150.f });

    scoreText = std::make_unique<sf::Text>(font, "Your score was: " + std::to_string(score), 50);
    scoreText->setFillColor(sf::Color::Black);
    scoreText->setOrigin(scoreText->getGlobalBounds().getCenter());
    scoreText->setPosition({ winSize.x / 2.f, 350.f });

    MainMenuSprite = std::make_unique<sf::Sprite>(buttonTex);
    MainMenuSprite->setOrigin(MainMenuSprite->getGlobalBounds().getCenter());
    MainMenuSprite->setScale({ 0.7f, 0.6f });
    MainMenuSprite->setPosition({winSize.x / 2.f - 200.f, winSize.y - 150.f });

    MainMenuText = std::make_unique<sf::Text>(font, "Main Menu", 25);
    MainMenuText->setFillColor(sf::Color::Black);
    MainMenuText->setOrigin(MainMenuText->getGlobalBounds().getCenter());
    MainMenuText->setPosition(MainMenuSprite->getPosition());

    PlayAgainSprite = std::make_unique<sf::Sprite>(buttonTex);
    PlayAgainSprite->setOrigin(PlayAgainSprite->getGlobalBounds().getCenter());
    PlayAgainSprite->setScale({ 0.7f, 0.6f });
    PlayAgainSprite->setPosition({winSize.x / 2.f + 200.f, winSize.y - 150.f});

    PlayAgainText = std::make_unique<sf::Text>(font, "Play Again", 25);
    PlayAgainText->setFillColor(sf::Color::Black);
    PlayAgainText->setOrigin(PlayAgainText->getGlobalBounds().getCenter());
    PlayAgainText->setPosition(PlayAgainSprite->getPosition());
}

void LoseScene::OnEnterScene()
{
	AudioManager::Get().PlayMusic("MenuMusic", true);
}

void LoseScene::OnExitScene()
{
	AudioManager::Get().StopMusic();
}

void LoseScene::HandleEvent(const sf::Event& event)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if((MainMenuSprite && MainMenuSprite->getGlobalBounds().contains(worldPos)))
            {
                RequestSceneChange("MainMenu");
            }
            else if (PlayAgainSprite && PlayAgainSprite->getGlobalBounds().contains(worldPos))
            {
                sceneManager.SwitchScene(std::make_unique<GameplayScene>(window, resources, sceneManager));
            }
        }
    }
}

void LoseScene::Update(float dt)
{
    sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    if (MainMenuSprite && MainMenuSprite->getGlobalBounds().contains(mousePos))
        MainMenuText->setFillColor(sf::Color::Green);
    else
        MainMenuText->setFillColor(sf::Color::Black);

    if (PlayAgainSprite && PlayAgainSprite->getGlobalBounds().contains(mousePos))
        PlayAgainText->setFillColor(sf::Color::Green);
    else
        PlayAgainText->setFillColor(sf::Color::Black);
}

void LoseScene::RenderScene()
{
    if (title) window.draw(*title);
    if (MainMenuSprite) window.draw(*MainMenuSprite);
    if (MainMenuText)   window.draw(*MainMenuText);
    if (scoreText) window.draw(*scoreText);
    if (PlayAgainSprite) window.draw(*PlayAgainSprite);
    if (PlayAgainText)   window.draw(*PlayAgainText);
}
