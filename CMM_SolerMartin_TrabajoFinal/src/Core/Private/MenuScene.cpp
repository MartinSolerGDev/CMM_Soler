//c++
#include <iostream>
//sfml
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

//Custom
#include "MenuScene.h"
#include "GameplayScene.h"
#include <CreditScene.h>
#include <SettingsScene.h>

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

   //Buttons
    buttons.push_back(CreateButton("PLAY", winSize.y / 2 - 50.f, [&] {manager.PushScene(std::make_unique<GameplayScene>(window, resources, manager));}));
    buttons.push_back(CreateButton("SETTINGS", winSize.y / 2 + 50, [&] {manager.PushScene(std::make_unique<SettingsScene>(window, resources, manager, false));}));
    buttons.push_back(CreateButton("CREDITS", winSize.y / 2 + 150.f, [&] {manager.PushScene(std::make_unique<CreditScene>(window, resources, manager)); }));
    buttons.push_back(CreateButton("QUIT", winSize.y / 2 + 250.f, [&] {window.close();}));
    
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

            for (auto& b : buttons)
            {
                if (b.buttonSprite && b.buttonSprite->getGlobalBounds().contains(worldPos))
                {
                    if (b.onClick) b.onClick();
                    break;
                }
            }
        }
    }
}

void MenuScene::Update(float deltaTime)
{
    sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

    for (auto& b : buttons)
    {
        if (b.buttonSprite && b.buttonSprite->getGlobalBounds().contains(mousePos))
        {
            if (b.buttonTxt)
                b.buttonTxt->setFillColor(sf::Color::Green);
        }
        else
        {
            if (b.buttonTxt)
                b.buttonTxt->setFillColor(sf::Color::Black); 
        }
    }
}

void MenuScene::RenderScene()
{
    if (title) window.draw(*title);
    for (auto& b : buttons)
    {
        if (b.buttonSprite) window.draw(*b.buttonSprite);
        if (b.buttonTxt)   window.draw(*b.buttonTxt);
    }

    if (enterToContinue) window.draw(*enterToContinue);
}

void MenuScene::OnEnterScene()
{
    AudioManager::Get().PlayMusic("MenuMusic", true);
}

void MenuScene::OnExitScene()
{
    AudioManager::Get().StopMusic();
}

Button MenuScene::CreateButton(const std::string& label, float y, std::function<void()> onClick)
{
    Button b;
    auto& buttonTex = resources.GetTexture("../assets/Background/buttonBackground.png",
        sf::IntRect({ 0,0 }, { 400,250 }));

    b.buttonSprite = std::make_unique<sf::Sprite>(buttonTex);
    b.buttonSprite->setOrigin(b.buttonSprite->getGlobalBounds().getCenter());
    b.buttonSprite->setScale({ 0.75f, 0.5f });
    b.buttonSprite->setPosition({ window.getSize().x / 2.f, y });

    b.buttonTxt = std::make_unique<sf::Text>(font, label, 30);
    b.buttonTxt->setFillColor(sf::Color::Black);
    b.buttonTxt->setOrigin(b.buttonTxt->getLocalBounds().getCenter());
    b.buttonTxt->setPosition({ window.getSize().x / 2.f, y });

    b.onClick = std::move(onClick);
    return b;

}