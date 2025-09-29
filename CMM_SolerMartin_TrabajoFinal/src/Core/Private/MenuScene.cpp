//c++
#include <iostream>
//sfml
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

//Custom
#include "MenuScene.h"
#include "GameplayScene.h"

MenuScene::MenuScene(sf::RenderWindow& window, ResourcesManager& resources, SceneManager& manager)
    : Scene(window), resources(resources), manager(manager), font(resources.GetFont("../assets/font.ttf"))
{
    SetBackground(resources.GetTexture("../assets/Background/menubackground.png", sf::IntRect({ 0,0 }, { 1280,720 })));
    sf::Font& font = resources.GetFont("../assets/font.ttf");

    sf::Vector2f winSize(window.getSize());
    title = std::make_unique<sf::Text>(font, "Quack Attack: Reloaded", 60);
    title->setFillColor(sf::Color::Black);
    title->setOrigin(title->getLocalBounds().getCenter());
    title->setPosition({ winSize.x / 2,150.f});


    buttonSprite = std::make_unique<sf::Sprite>(resources.GetTexture("../assets/Background/buttonBackground.png", sf::IntRect({ 0,0 }, { 400, 250 })));
    buttonSprite->setOrigin(buttonSprite->getGlobalBounds().getCenter());
    buttonSprite->setScale({ 0.75f, 0.5f });
    buttonSprite->setPosition({ winSize.x / 2, winSize.y / 2 });
    buttonTxt = std::make_unique<sf::Text>(font, "PLAY", 35);
    buttonTxt->setFillColor(sf::Color::Black);
    buttonTxt->setOrigin(buttonTxt->getLocalBounds().getCenter());
    buttonTxt->setPosition({ winSize.x / 2, winSize.y / 2});

    enterToContinue = std::make_unique<sf::Text>(font, "Press play or enter to continue", 25);
    enterToContinue->setFillColor(sf::Color::Black);
    enterToContinue->setOrigin(enterToContinue->getLocalBounds().getCenter());
    enterToContinue->setPosition({ winSize.x / 2, winSize.y / 2 - 150 });
}

void MenuScene::HandleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
        { 
            manager.PushScene(std::make_unique<GameplayScene>(window, resources, manager));
        }
    }
    else if (const auto* mousePressed = event.getIf < sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            if (buttonSprite->getGlobalBounds().contains(worldPos))
            {
                manager.PushScene(std::make_unique<GameplayScene>(window, resources, manager));
            }
        }
    }
}

void MenuScene::Update(float deltaTime) {}

void MenuScene::RenderScene()
{
    if (title) window.draw(*title);
    if (buttonSprite) window.draw(*buttonSprite);
    if (buttonTxt) window.draw(*buttonTxt);
    if (enterToContinue) window.draw(*enterToContinue);
}


